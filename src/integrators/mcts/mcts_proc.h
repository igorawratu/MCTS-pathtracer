#ifndef MCTS_PROC_H_
#define MCTS_PROC_H_

#include <mitsuba/core/sched.h>
#include <mitsuba/core/plugin.h>
#include <mitsuba/render/range.h>
#include <mitsuba/render/rectwu.h>
#include <mitsuba/render/imageblock.h>
#include <mitsuba/render/film.h>
#include <mitsuba/render/scene.h>
#include <mitsuba/render/sampler.h>
#include <mitsuba/render/sensor.h>
#include <mitsuba/core/statistics.h>
#include <vector>

MTS_NAMESPACE_BEGIN

class MCTSWorkProcessor : public WorkProcessor {
public:
    MCTSWorkProcessor();

    MCTSWorkProcessor(Stream *stream, InstanceManager *manager);

    void serialize(Stream *stream, InstanceManager *manager) const;

    ref<WorkUnit> createWorkUnit() const;

    ref<WorkResult> createWorkResult() const;

    void prepare();

    void process(const WorkUnit *workUnit, WorkResult *workResult, const bool &stop);

    ref<WorkProcessor> clone() const;

    MTS_DECLARE_CLASS()
private:
    ref<Sensor> sensor_;
    ref<Film> film_;
    ref<Scene> scene_;
    ref<Sampler> sampler_;
};


class MCTSProcess : public ParallelProcess {
public:
    MCTSProcess(const RenderJob *parent, RenderQueue *queue, const std::vector<std::pair<Point2i, Vector2i>>& image_blocks);

    void processResult(const WorkResult *wr, bool cancelled);
    ref<WorkProcessor> createWorkProcessor() const;
    void bindResource(const std::string &name, int id);
    EStatus generateWork(WorkUnit *unit, int worker);

    ImageBlock* accumulator(){return accumulator_;}

    MTS_DECLARE_CLASS()
protected:
    /// Virtual destructor
    virtual ~MCTSProcess();
private:
    std::vector<std::pair<Point2i, Vector2i>> image_blocks_;
    ref<ImageBlock> accumulator_;
    ProgressReporter* progress_;
    ref<Film> film_;
    std::uint32_t result_counter_, generate_counter_;
    ref<const RenderJob> renderjob_;
};




MTS_NAMESPACE_END

#endif