#include "simpleselectionpol.h"

MTS_NAMESPACE_BEGIN

const float CP = 0.70710678118f;

//figure out how to do probabilities as well
std::uint32_t SimpleSelectionPolicy::selectChild(MCTSTreeNode* current, const Intersection& its, float& probability){
    std::vector<std::uint32_t> unvisited_children;

    for(std::uint32_t i = 0; i < current->children.size(); ++i){
        if(current->children[i] == nullptr){
            unvisited_children.push_back(i);
        }
    }

    if(unvisited_children.size() > 0){
        return unvisited_children[rand() % unvisited_children.size()];
    }

    std::vector<float> probabilities(current->children.size());

    for(std::uint32_t i = 0; i < current->children.size(); ++i){
        float& p = probabilities[i];

        p = current->children[i]->val.getLuminance();
        p += 2.f * CP + sqrt(2.f * log(float(current->visited)) / float(current->children[i]->visited));
    }

    std::discrete_distribution<std::uint32_t> dist(probabilities.begin(), probabilities.end());

    return dist(rng_);
}

std::uint32_t SimpleSelectionPolicy::selectSensorChild(MCTSTreeNode* current, float& probability){
    std::vector<std::uint32_t> unvisited_children;

    for(std::uint32_t i = 0; i < current->children.size(); ++i){
        if(current->children[i] == nullptr){
            unvisited_children.push_back(i);
        }
    }

    if(unvisited_children.size() > 0){
        return unvisited_children[rand() % unvisited_children.size()];
    }

    std::vector<float> probabilities(current->children.size());

    for(std::uint32_t i = 0; i < current->children.size(); ++i){
        float& p = probabilities[i];

        p = current->children[i]->val.getLuminance();
        p += 2.f * CP + sqrt(2.f * log(float(current->visited)) / float(current->children[i]->visited));
    }

    std::discrete_distribution<std::uint32_t> dist(probabilities.begin(), probabilities.end());

    return dist(rng_);
}

MTS_NAMESPACE_END