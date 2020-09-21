#ifndef MCTSTREE_H_
#define MCTSTREE_H_

#include <mitsuba/core/plugin.h>
#include <vector>
#include <mutex>
#include <memory>

MTS_NAMESPACE_BEGIN

class MCTSTreeNode{
public:
    MCTSTreeNode() = delete;
    MCTSTreeNode(std::uint32_t num_children);
    MCTSTreeNode(const MCTSTreeNode& other) = delete;
    ~MCTSTreeNode();

    std::vector<std::unique_ptr<MCTSTreeNode>> children;
    float prob_acc;
    Spectrum val;
    std::uint32_t visited;
};

class SelectionPolicy{
public:
    virtual std::uint32_t selectChild(MCTSTreeNode* current, const Intersection& its, float& probability) = 0;
};

class DefaultPolicy{
public:
    virtual Spectrum simulate(Intersection& its, Vector3f& wo, Sampler* sampler, Scene* scene) = 0;
};

class NodeDiscretizer{
public:
    virtual std::uint32_t getNumChildren(const std::pair<Point2i, Vector2i>& sensor_area) = 0;
    virtual std::uint32_t getNumChildren(const Intersection& its) = 0;
    virtual std::pair<Vector2f, Vector2f> getChildItsCoordRange(const Intersection& its, std::uint32_t child) = 0;
    virtual Point2i getChildSensorCoord(const std::pair<Point2i, Vector2i>& sensor_area, std::uint32_t child, std::pair<Point2, Vector2f>& sub_area) = 0;
};

class PathGenerator{
public:
    virtual std::vector<Intersection> generatePath(const std::pair<Point2i, Vector2i>& sensor_area, 
        MCTSTreeNode* root, SelectionPolicy* spol, NodeDiscretizer* ndisc, std::vector<std::uint32_t>& child_indices,
        std::vector<float>& probabilities, Sampler* sampler, Scene* scene) = 0;
};

class ContributionCalculator{
public:
    virtual Spectrum computeContribution(Intersection& its, Vector3f wo, Spectrum incoming) = 0;
};

class MCTSTree{
public:
    MCTSTree() = delete;
    MCTSTree(const std::pair<Point2i, Vector2i>& sensor_area, ImageBlock* wr, std::unique_ptr<SelectionPolicy> spol, 
        std::unique_ptr<DefaultPolicy> dpol, std::unique_ptr<PathGenerator> pathgen, std::unique_ptr<ContributionCalculator> contrib, 
        std::unique_ptr<NodeDiscretizer> ndisc, Sampler* sampler, Scene* scene);
    ~MCTSTree();

    void iterate();
    void develop(std::mutex& wr_mutex);
private:
    std::pair<Point2i, Vector2i> sensor_area_;
    ImageBlock* wr_;
    std::unique_ptr<SelectionPolicy> spol_;
    std::unique_ptr<DefaultPolicy> dpol_;
    std::unique_ptr<PathGenerator> pathgen_;
    std::unique_ptr<ContributionCalculator> contrib_;
    std::unique_ptr<NodeDiscretizer> ndisc_;

    std::unique_ptr<MCTSTreeNode> root_;

    Sampler* sampler_;
    Scene* scene_;
};

MTS_NAMESPACE_END

#endif