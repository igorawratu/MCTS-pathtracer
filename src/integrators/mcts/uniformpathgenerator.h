#ifndef UNIFORMPATHGENERATOR_H_
#define UNIFORMPATHGENERATOR_H_

#include <mitsuba/core/plugin.h>
#include "mctstree.h"

MTS_NAMESPACE_BEGIN

class UniformPathGenerator : public PathGenerator{
public:
    UniformPathGenerator(){}
    ~UniformPathGenerator(){}

    std::vector<Intersection> generatePath(const std::pair<Point2i, Vector2i>& sensor_area, 
        MCTSTreeNode* root, SelectionPolicy* spol, NodeDiscretizer* ndisc, std::vector<std::uint32_t>& child_indices,
        std::vector<float>& probabilities, Sampler* sampler, Scene* scene);
};

MTS_NAMESPACE_END

#endif