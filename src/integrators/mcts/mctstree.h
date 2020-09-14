#ifndef MCTSTREE_H_
#define MCTSTREE_H_

#include <mitsuba/core/plugin.h>
#include <vector>

MTS_NAMESPACE_BEGIN

template <class ChildSelectorType> 
class MCTSTreeNode{
public:
    MCTSTreeNode();
    MCTSTreeNode(int num_children);
    MCTSTreeNode(const MCTSTreeNode& other);
    ~MCTSTreeNode();

    Spectrum sample();

private:
    ChildSelectorType child_selector_;
    std::vector<std::pair<std::unique_ptr<MCTSTreeNode>, float>> children_;
}

MTS_NAMESPACE_END

#endif