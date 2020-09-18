#ifndef SIMPLENODEDISCRETIZER_H_
#define SIMPLENODEDISCRETIZER_H_

#include "mctstree.h"
#include <mitsuba/core/plugin.h>

MTS_NAMESPACE_BEGIN

class SimpleNodeDiscretizer : NodeDiscretizer{
public:
    SimpleNodeDiscretizer() : SimpleNodeDiscretizer(Point2i(1)){}
    SimpleNodeDiscretizer(Point2i hres) : hemisphere_resolution_(hres){
        hemisphere_range_ = Vector2f(1.f / hemisphere_resolution_.x, 1.f / hemisphere_resolution_.y);
    }
    ~SimpleNodeDiscretizer(){}
    
    std::uint32_t getNumChildren(const std::pair<Point2i, Vector2i>& sensor_area);
    std::uint32_t getNumChildren(const Intersection& its);
    std::pair<Vector2f, Vector2f> getChildItsCoordRange(const Intersection& its, std::uint32_t child);
    Point2i getChildSensorCoord(const std::pair<Point2i, Vector2i>& sensor_area, std::uint32_t child, std::pair<Point2, Vector2f>& sub_area);

private:
    Point2i hemisphere_resolution_;
    Vector2f hemisphere_range_;
};

MTS_NAMESPACE_END

#endif