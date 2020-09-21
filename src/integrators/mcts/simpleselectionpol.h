#ifndef SIMPLESELECTIONPOL_H_
#define SIMPLESELECTIONPOL_H_

#include "mctstree.h"
#include <mitsuba/core/plugin.h>
#include <random>
#include <chrono>

MTS_NAMESPACE_BEGIN

class SimpleSelectionPolicy : SelectionPolicy{
public:
    SimpleSelectionPolicy() : rng_(std::chrono::high_resolution_clock::now().time_since_epoch().count()){}
    ~SimpleSelectionPolicy(){}

    std::uint32_t selectChild(MCTSTreeNode* current, const Intersection& its, float& probability);

private:
    std::mt19937 rng_;
};

MTS_NAMESPACE_END

#endif