#include "mcts.h"

#include <mitsuba/bidir/util.h>
#include <mitsuba/core/fstream.h>
#include <vector>

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

    bool render(Scene *scene, RenderQueue *queue, const RenderJob *job, int sceneResID, int sensorResID, int samplerResID) {
        ref<Scheduler> scheduler = Scheduler::getInstance();
        ref<Sensor> sensor = scene->getSensor();
        ref<Sampler> sampler = sensor->getSampler();
        const Film *film = sensor->getFilm();

        Vector2i film_size = film->getCropSize();
        Vector2i block_size(64, 64);

        std::vector<std::pair<Point2i, Vector2i>> image_blocks;
        std::uint32_t x_blocks = film_size.x / block_size.x + std::min(film_size.x % block_size.x, 1);
        std::uint32_t y_blocks = film_size.y / block_size.y + std::min(film_size.y % block_size.y, 1);

        for(std::uint32_t y = 0; y < y_blocks; ++y){
            for(std::uint32_t x = 0; x < x_blocks; ++x){
                Point2i offset(x * film_size.x, y * film_size.y);
                Vector2i size(std::min(block_size.x, film_size.x - (offset.x + block_size.x)),
                    std::min(block_size.y, film_size.y - (offset.y + block_size.y)));
                image_blocks.emplace_back(offset, size);
            }
        }

        ref<MCTSProcess> process = new MCTSProcess(job, queue, image_blocks);

        process->bindResource("scene", sceneResID);
        process->bindResource("sensor", sensorResID);
        process->bindResource("sampler", samplerResID);

        scheduler->schedule(process);
        scheduler->wait(process);

        film->setBitmap((Spectrum *)process->accumulator()->getBitmap()->getData());

        return process->getReturnStatus() == ParallelProcess::ESuccess;
    }

    MTS_DECLARE_CLASS()
private:
    ref<ParallelProcess> process_;
    ref<RenderJob> job_;
};

MTS_IMPLEMENT_CLASS_S(MCTS, false, Integrator)
MTS_EXPORT_PLUGIN(MCTS, "Monte Carlo Tree Search Integrator");
MTS_NAMESPACE_END
