#include "mctstree.h"

MTS_NAMESPACE_BEGIN

MCTSTree::MCTSTree(const std::pair<Point2i, Vector2i>& sensor_area, ImageBlock* wr, std::unique_ptr<SelectionPolicy> spol, 
        std::unique_ptr<DefaultPolicy> dpol, std::unique_ptr<PathGenerator> pathgen, std::unique_ptr<ContributionCalculator> contrib, 
        std::unique_ptr<NodeDiscretizer> ndisc){

}

MCTSTree::~MCTSTree(){

}

void MCTSTree::iterate(std::mutex& wr_mutex){
    
}

MTS_NAMESPACE_END