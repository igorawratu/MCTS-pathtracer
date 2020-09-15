#ifndef MCTSTREE_H_
#define MCTSTREE_H_

#include <mitsuba/core/plugin.h>
#include <vector>
#include <mutex>
#include <memory>

MTS_NAMESPACE_BEGIN

class MCTSTreeNode{
public:
    MCTSTreeNode();
    MCTSTreeNode(int num_children);
    MCTSTreeNode(const MCTSTreeNode& other);
    ~MCTSTreeNode();

    std::vector<std::pair<std::unique_ptr<MCTSTreeNode>, float>> children_;
};

class SelectionPolicy{
public:
    virtual std::uint32_t selectChild(MCTSTreeNode* current) = 0;
};

class DefaultPolicy{
public:
    virtual Spectrum simulate(Intersection& its) = 0;
};

class NodeDiscretizer{
public:
    virtual std::uint32_t getNumChildren(const std::pair<Point2i, Vector2i>& sensor_area) = 0;
    virtual std::uint32_t getNumChildren(const Intersection& its) = 0;
    virtual std::pair<Vector2f, Vector2f> getChildItsCoordRange(const Intersection& its, std::uint32_t child) = 0;
    virtual std::pair<Vector2i, Vector2i> getChildSensorCoordRange(const std::pair<Point2i, Vector2i>& sensor_area, std::uint32_t child) = 0;
};

class PathGenerator{
public:
    virtual std::vector<Intersection> GeneratePath(const std::pair<Point2i, Vector2i>& sensor_area, 
        MCTSTreeNode* root, SelectionPolicy* spol, NodeDiscretizer* ndisc) = 0;
};

class ContributionCalculator{
public:
    virtual Spectrum computeContribution(Intersection& its, Spectrum incoming) = 0;
};

class MCTSTree{
public:
    MCTSTree() = delete;
    MCTSTree(const std::pair<Point2i, Vector2i>& sensor_area, ImageBlock* wr, std::unique_ptr<SelectionPolicy> spol, 
        std::unique_ptr<DefaultPolicy> dpol, std::unique_ptr<PathGenerator> pathgen, std::unique_ptr<ContributionCalculator> contrib, 
        std::unique_ptr<NodeDiscretizer> ndisc);
    ~MCTSTree();

    void iterate(std::mutex& wr_mutex);
private:
    std::pair<Point2i, Vector2i> sensor_area_;
    ImageBlock* wr_;
    std::unique_ptr<SelectionPolicy> spol_;
    std::unique_ptr<DefaultPolicy> dpol_;
    std::unique_ptr<PathGenerator> pathgen_;
    std::unique_ptr<ContributionCalculator> contrib_;
    std::unique_ptr<NodeDiscretizer> ndisc_;
};

MTS_NAMESPACE_END

#endif