#include "simplecontributioncalculator.h"

#include <mitsuba/render/shape.h>
#include <mitsuba/render/bsdf.h>

MTS_NAMESPACE_BEGIN

Spectrum SimpleContributionCalculator::computeContribution(Intersection& its, Vector3f wo, Spectrum incoming){
    Spectrum contrib(0.f);

    if(!its.isValid()){
        return contrib;
    }

    BSDFSamplingRecord bsrec(its, its.toLocal(wo));
    contrib = its.getBSDF()->eval(bsrec);

    if(its.isEmitter()){
        contrib += its.Le(its.wi);
    }

    return contrib;
}

MTS_NAMESPACE_END