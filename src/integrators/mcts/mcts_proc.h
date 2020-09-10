#ifndef MCTS_PROC_H_
#define MCTS_PROC_H_

#include <mitsuba/core/sched.h>
#include <mitsuba/core/plugin.h>
#include <mitsuba/render/range.h>

MTS_NAMESPACE_BEGIN

class MCTSWorkProcessor : public WorkProcessor {
public:
    MCTSWorkProcessor();

    MCTSWorkProcessor(Stream *stream, InstanceManager *manager) : WorkProcessor(stream, manager);

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
    MCTSProcess(const RenderJob *parent, RenderQueue *queue,
        const MLTConfiguration &config, const Bitmap *directImage,
        const std::vector<PathSeed> &seeds);

    void develop();

    /* ParallelProcess impl. */
    void processResult(const WorkResult *wr, bool cancelled);
    ref<WorkProcessor> createWorkProcessor() const;
    void bindResource(const std::string &name, int id);
    EStatus generateWork(WorkUnit *unit, int worker);

    MTS_DECLARE_CLASS()
protected:
    /// Virtual destructor
    virtual ~MCTSProcess() { }
private:

};




MTS_NAMESPACE_END

#endif