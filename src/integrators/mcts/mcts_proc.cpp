#include "mcts_proc.h"

MTS_NAMESPACE_BEGIN

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
    film_ = sensor_->getFilm();

    scene_->setSensor(sensor_);
    scene_->setSampler(sampler_);
    scene_->removeSensor(scene->getSensor());
    scene_->addSensor(sensor_);
    scene_->setSensor(sensor_);
    scene_->wakeup(NULL, m_resources);
    scene_->initializeBidirectional();
}

void MCTSWorkProcessor::process(const WorkUnit *workUnit, WorkResult *workResult, const bool &stop){
    //actual rendering is done here
}

ref<WorkProcessor> MCTSWorkProcessor::clone() const{
    return new MCTSWorkProcessor();
}


MCTSProcess::MCTSProcess(const RenderJob *parent, RenderQueue *queue, const std::vector<std::pair<Point2i, Vector2i>>& image_blocks) : 
    result_counter_(0),
    generate_counter_(0),
    renderjob_(parent){

}

MCTSProcess::~MCTSProcess(){
    delete progress_;
}

void MCTSProcess::processResult(const WorkResult *wr, bool cancelled){
    const ImageBlock *result = static_cast<const ImageBlock *>(wr);
    accumulator_->put(result);
    progress_->update(++result_counter_);
}

ref<WorkProcessor> MCTSProcess::createWorkProcessor() const{
    return new MCTSWorkProcessor();
}

void MCTSProcess::bindResource(const std::string &name, int id){
    ParallelProcess::bindResource(name, id);
    if (name == "sensor") {
        film_ = static_cast<Sensor *>(Scheduler::getInstance()->getResource(id))->getFilm();
        if (progress_)
            delete progress_;
        progress_ = new ProgressReporter("Rendering", image_blocks_.size(), renderjob_);
        accumulator_ = new ImageBlock(Bitmap::ESpectrum, film_->getCropSize());
        accumulator_->clear();
    }
}

ParallelProcess::EStatus MCTSProcess::generateWork(WorkUnit *unit, int worker){
    if (generate_counter_ >= image_blocks_.size())
        return EFailure;

    auto rect = image_blocks_[generate_counter_++];
    RectangularWorkUnit* work_unit = static_cast<RectangularWorkUnit*>(unit);
    work_unit->setSize(rect.second);
    work_unit->setOffset(rect.first);

    return ESuccess;
}

MTS_NAMESPACE_END