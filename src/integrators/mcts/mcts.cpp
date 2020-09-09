#include <mitsuba/bidir/util.h>
#include <mitsuba/core/fstream.h>
#include "mlt_proc.h"

MTS_NAMESPACE_BEGIN

class MCTS : public Integrator {
public:
    MCTS(const Properties &props) : Integrator(props) {
    }

    /// Unserialize from a binary data stream
    MCTS(Stream *stream, InstanceManager *manager) : Integrator(stream, manager) {
    }

    virtual ~MCTS() { }

    void serialize(Stream *stream, InstanceManager *manager) const {
        Integrator::serialize(stream, manager);
    }

    bool preprocess(const Scene *scene, RenderQueue *queue,
            const RenderJob *job, int sceneResID, int sensorResID,
            int samplerResID) {
        Integrator::preprocess(scene, queue, job, sceneResID,
                sensorResID, samplerResID);

        return true;
    }

    void cancel() {
    }

    bool render(Scene *scene, RenderQueue *queue, const RenderJob *job,
            int sceneResID, int sensorResID, int samplerResID) {
        
    }

    MTS_DECLARE_CLASS()
private:

};

MTS_IMPLEMENT_CLASS_S(MCTS, false, Integrator)
MTS_EXPORT_PLUGIN(MCTS, "Monte Carlo Tree Search Integrator");
MTS_NAMESPACE_END
