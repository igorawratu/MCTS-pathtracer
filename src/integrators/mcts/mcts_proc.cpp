#include "mcts_proc.h"

#include <mitsuba/render/rectwu.h>
#include <mitsuba/render/imageblock.h>

MCTSWorkProcessor::MCTSWorkProcessor(){

}

MCTSWorkProcessor::MCTSWorkProcessor(Stream *stream, InstanceManager *manager) : WorkProcessor(stream, manager){

}

void MCTSWorkProcessor::serialize(Stream *stream, InstanceManager *manager) const{

}

ref<WorkUnit> MCTSWorkProcessor::createWorkUnit() const{
    return new RectangularWorkUnit();
}

ref<WorkResult> MCTSWorkProcessor::createWorkResult() const{
    return new ImageBlock(Bitmap::ESpectrum, film_->getCropSize(), film_->getReconstructionFilter());
}

void MCTSWorkProcessor::prepare(){
    Scene *scene = static_cast<Scene*>(getResource("scene"));
    sampler_ = static_cast<Sampler *>(getResource("sampler"));
    sensor_ = static_cast<Sensor *>(getResource("sensor"));
    scene_ = new Scene(scene);
    film_ = sensor->getFilm();

    scene_->setSensor(sensor_);
    scene_->setSampler(sampler_);
    scene_->removeSensor(scene->getSensor());
    scene_->addSensor(sensor_);
    scene_->setSensor(sensor_);
    scene_->wakeup(NULL, m_resources);
    scene_->initializeBidirectional();
}

void MCTSWorkProcessor::process(const WorkUnit *workUnit, WorkResult *workResult, const bool &stop){

}

ref<WorkProcessor> MCTSWorkProcessor::clone() const{
    return new MCTSWorkProcessor();
}


MCTSProcess::MCTSProcess(const RenderJob *parent, RenderQueue *queue,
        const MLTConfiguration &config, const Bitmap *directImage,
        const std::vector<PathSeed> &seeds){

}

void MCTSProcess::develop(){

}

void MCTSProcess::processResult(const WorkResult *wr, bool cancelled){

}

ref<WorkProcessor> MCTSProcess::createWorkProcessor() const{

}

void MCTSProcess::bindResource(const std::string &name, int id){

}

EStatus MCTSProcess::generateWork(WorkUnit *unit, int worker){
    
}