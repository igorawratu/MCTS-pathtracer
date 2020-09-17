#ifndef SIMPLECONTRIBUTIONCALCULATOR_H_
#define SIMPLECONTRIBUTIONCALCULATOR_H_

#include <mitsuba/core/plugin.h>
#include "mctstree.h"

MTS_NAMESPACE_BEGIN

class SimpleContributionCalculator : public ContributionCalculator{
public:
    SimpleContributionCalculator(){}
    ~SimpleContributionCalculator(){}

    Spectrum computeContribution(Intersection& its, Vector3f wo, Spectrum incoming);
};

MTS_NAMESPACE_END

#endif