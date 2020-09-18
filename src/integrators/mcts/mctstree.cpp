#include "mctstree.h"

#include <stack>
#include <unordered_map>
#include <vector>

MTS_NAMESPACE_BEGIN

MCTSTreeNode::MCTSTreeNode(std::uint32_t num_children) : 
    children(num_children, nullptr),
    prob_acc(0.f),
    val(0.f),
    visited(0){

}

MCTSTreeNode::~MCTSTreeNode(){

}

MCTSTree::MCTSTree(const std::pair<Point2i, Vector2i>& sensor_area, ImageBlock* wr, std::unique_ptr<SelectionPolicy> spol, 
        std::unique_ptr<DefaultPolicy> dpol, std::unique_ptr<PathGenerator> pathgen, std::unique_ptr<ContributionCalculator> contrib, 
        std::unique_ptr<NodeDiscretizer> ndisc, Sampler* sampler, Scene* scene) : 
            sensor_area_(sensor_area), 
            wr_(wr), 
            spol_(std::move(spol)), 
            dpol_(std::move(dpol)),
            pathgen_(std::move(pathgen)),
            contrib_(std::move(contrib)),
            ndisc_(std::move(ndisc)),
            sampler_(sampler),
            scene_(scene){
    std::uint32_t root_children = ndisc_->getNumChildren(sensor_area_);
    root_ = std::unique_ptr<MCTSTreeNode>(new MCTSTreeNode(root_children));

}

MCTSTree::~MCTSTree(){

}

void MCTSTree::iterate(){
    std::vector<std::uint32_t> visited_children;
    std::vector<float> probabilities;

    //selection and expansion
    std::vector<Intersection> path = pathgen_->generatePath(sensor_area_, root_.get(), spol_.get(), 
        ndisc_.get(), visited_children, probabilities, sampler_, scene_);

    //simulate from last node to obtain result
    Vector3f wo;
    Spectrum simulated = dpol_->simulate(path.back(), wo, sampler_, scene_);
    std::stack<MCTSTreeNode*> visited_nodes;

    //backpropagation to update stats
    MCTSTreeNode* curr = root_.get();
    for(std::uint32_t i = 0; i < visited_children.size(); ++i){
        curr = curr->children[visited_children[i]];
        visited_nodes.push(curr);
    }

    while(!visited_nodes.empty()){
        curr = visited_nodes.top();
        visited_nodes.pop();
        Intersection curr_its = path.back();
        simulated = contrib_->computeContribution(path.back(), wo, simulated);
        path.pop_back();
        curr->prob_acc += probabilities.back();
        probabilities.pop_back();
        curr->val += simulated;
        curr->visited++;
        
        //update wo if we have not yet processed all nodes
        if(path.size() > 0){
            wo = Vector3f(normalize(curr_its.p - path.back().p));
        }
    }
}

void MCTSTree::develop(std::mutex& wr_mutex){
    Vector2i size = wr_->getSize();
    std::unordered_map<std::uint32_t, std::uint32_t> total_sampled;
    std::unordered_map<std::uint32_t, Spectrum> final_pixel_values;

    std::uint32_t child, std::pair<Point2, Vector2f> sub_area;

    for(std:uint32_t i = 0; i < root_->children.size(); ++i){
        if(root_->children[i] == nullptr){
            continue;
        }
        Point2i pixel = ndisc_->getChildSensorCoord(sensor_area_, i, sub_area);
        std::uint32_t pixel_idx = pixel.x + pixel.y * size.x;

        if(total_sampled.find(pixel_idx) != total_sampled.end()){
            total_sampled[pixel_idx] += root_->children[i]->visited;
        }
        else total_sampled[pixel_idx] = root_->children[i]->visited;
    }

    for(std:uint32_t i = 0; i < root_->children.size(); ++i){
        if(root_->children[i] == nullptr){
            continue;
        }

        Point2i pixel = ndisc_->getChildSensorCoord(sensor_area_, i, sub_area);
        std::uint32_t pixel_idx = pixel.x + pixel.y * size.x;

        Spectrum child_contrib = root_->children[i]->val / root_->children[i]->prob_accum * 
            ((float)root_->children[i]->visited / total_sampled[pixel_idx]);

        if(final_pixel_values.find(pixel_idx) != final_pixel_values.end()){
            final_pixel_values[pixel_idx] += child_contrib;
        }
        else final_pixel_values[pixel_idx] = child_contrib;
    }

    {
        std::lock_guard<std::mutex> lock(wr_mutex);

        for(auto iter = final_pixel_values.begin(); iter != final_pixel_values.end(); ++iter){
            Point2i pixel(iter->first % size.x, iter->first / size.x);
            wr_->put(pixel, iter->second);
        }
    }
}

MTS_NAMESPACE_END