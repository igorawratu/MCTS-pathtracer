#include "uniformpathgenerator.h"

MTS_NAMESPACE_BEGIN

std::vector<Intersection> UniformPathGenerator::generatePath(const std::pair<Point2i, Vector2i>& sensor_area, 
        MCTSTreeNode* root, SelectionPolicy* spol, NodeDiscretizer* ndisc, std::vector<std::uint32_t>& child_indices,
        std::vector<float>& probabilities, Sampler* sampler, Scene* scene){
    
    std::uint32_t child = spol->selectChild(root);
    auto pixel_coords = ndisc->getChildSensorCoord(sensor_area, child);

    Point2 offset = sampler->next2D();
    offset.x -= 0.5f;
    offset.y -= 0.5f;

    
}

MTS_NAMESPACE_END