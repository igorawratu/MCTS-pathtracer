#include "bsdfdefaultpolicy.h"


MTS_NAMESPACE_BEGIN

Spectrum BSDFDefaultPolicy::simulate(Intersection& its, Vector3f& wo, Sampler* sampler, Scene* scene){
    //does not matter as incoming does not get accounted here
    if(!curr_its.isValid() || curr_its.isEmitter()){
        return Spectrum(0.f);
    }

    std::uint32_t vert_count = 0;
    Intersection curr_its = its;
    Spectrum col(1.f);

    while(vert_count++ < max_verts_ && curr_its.isValid() && !curr_its.isEmitter()){
        BSDFSamplingRecord bsr(curr_its, sampler);
        col *= bsdf->sample(bsdf_sample_record, sampler->next2D());

        Ray ray(curr_its.p, bsr.its.toWorld(bsr.wo), curr_its.time);
        scene->rayIntersect(ray, curr_its);
    }

    if(curr_its.isEmitter()){
        col *= curr_its.Le(curr_its.wi);
    }
    else col *= Spectrum(0.f);

    return col;
}

MTS_NAMESPACE_END