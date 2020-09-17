#include "simplecontributioncalculator.h"

MTS_NAMESPACE_BEGIN

Spectrum SimpleContributionCalculator::computeContribution(Intersection& its, Vector3f wo, Spectrum incoming){
    BSDFSamplingRecord bsrec(its, its.toLocal(wo));
    return its.getBSDF()->eval(bsrec);
}

MTS_NAMESPACE_END