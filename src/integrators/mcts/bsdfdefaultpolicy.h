#ifndef BSDFDEFAULTPOLICY_H_
#define BSDFDEFAULTPOLICY_H_

#include "mctstree.h"
#include <mitsuba/core/plugin.h>

MTS_NAMESPACE_BEGIN

class BSDFDefaultPolicy : DefaultPolicy{
public:
    BSDFDefaultPolicy() : BSDFDefaultPolicy(2){}
    BSDFDefaultPolicy(std::uint32_t max_verts) : max_verts_(max_verts){}
    ~BSDFDefaultPolicy(){}

    Spectrum simulate(Intersection& its, Vector3f& wo, Sampler* sampler, Scene* scene);

private:
    std::uint32_t max_verts_;
};

MTS_NAMESPACE_END

#endif