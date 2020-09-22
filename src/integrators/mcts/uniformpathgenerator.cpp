#include "uniformpathgenerator.h"
#include <mitsuba/render/scene.h>

MTS_NAMESPACE_BEGIN

std::vector<Intersection> UniformPathGenerator::generatePath(const std::pair<Point2i, Vector2i>& sensor_area, 
        MCTSTreeNode* root, SelectionPolicy* spol, NodeDiscretizer* ndisc, std::vector<std::uint32_t>& child_indices,
        std::vector<float>& probabilities, Sampler* sampler, Scene* scene, bool sample_time, bool sample_aperture){
    std::vector<Intersection> path_verts;

    ref<Sensor> sensor = scene->getSensor();

    float prob;
    std::uint32_t child = spol->selectSensorChild(root, prob);

    std::pair<Point2, Vector2f> sub_area;
    Point2i pixel_coords = ndisc->getChildSensorCoord(sensor_area, child, sub_area);

    Point2 sampled = sampler->next2D();
    Point2 offset = sub_area.first + Point2(float(sub_area.second.x) * 
        sampled.x, float(sub_area.second.y) * sampled.y);
    Point2 sensor_coord = Point2(pixel_coords) + offset;

    Point2 aperture_sample = sample_aperture ? sampler->next2D() : Point2(0.5f, 0.5f);
    Float time_sample = sample_time ? sampler->next1D() : 0.5f;

    Ray ray;
    sensor->sampleRay(ray, sensor_coord, aperture_sample, time_sample);
    
    Intersection its;
    scene->rayIntersect(ray, its);

    path_verts.push_back(its);
    probabilities.push_back(prob);
    child_indices.push_back(child);

    MCTSTreeNode* curr = root->children[child].get();
    MCTSTreeNode* last = root;

    while(curr != nullptr && its.isValid() && !its.isEmitter()){
        child = spol->selectChild(curr, its, prob);
        auto coord_range = ndisc->getChildItsCoordRange(its, child);
        Point2 sampled = sampler->next2D();
        Vector2f scoords = coord_range.first + Vector2f(sampled.x * coord_range.second.x, 
            sampled.y * coord_range.second.y);
        
        Vector3f wi(0.f);
        wi.x = sin(scoords.y) * cos(scoords.x);
        wi.y = sin(scoords.y) * sin(scoords.x);
        wi.z = cos(scoords.y);

        ray = Ray(its.p, its.toWorld(wi), ray.time);

        scene->rayIntersect(ray, its);

        last = curr;
        curr = curr->children[child].get();

        path_verts.push_back(its);
        probabilities.push_back(prob);
        child_indices.push_back(child);
    }

    if(curr == nullptr){
        std::uint32_t num_children = ndisc->getNumChildren(its);
        last->children[child] = std::unique_ptr<MCTSTreeNode>(new MCTSTreeNode(num_children));
    }

    return path_verts;
}

MTS_NAMESPACE_END