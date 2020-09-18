#include "simplenodediscretizer.h"

MTS_NAMESPACE_BEGIN

std::uint32_t SimpleNodeDiscretizer::getNumChildren(const std::pair<Point2i, Vector2i>& sensor_area){
    return sensor_area.second.x * sensor_area.second.y;
}

std::uint32_t SimpleNodeDiscretizer::getNumChildren(const Intersection& its){
    return hemisphere_resolution_.x * hemisphere_resolution_.y;
}

std::pair<Vector2f, Vector2f> SimpleNodeDiscretizer::getChildItsCoordRange(const Intersection& its, std::uint32_t child){
    assert(child < getNumChildren(its));

    std::uint32_t x_pos = child % hemisphere_resolution_.x;
    std::uint32_t y_pos = child / hemisphere_resolution_.y;

    Vector2f start(hemisphere_range_.x * x_pos, hemisphere_range_.y * y_pos);

    return std::make_pair(start, hemisphere_range_);
}

Point2i SimpleNodeDiscretizer::getChildSensorCoord(const std::pair<Point2i, Vector2i>& sensor_area, std::uint32_t child, 
    std::pair<Point2, Vector2f>& sub_area){
    assert(child < getNumChildren(sensor_area));

    std::uint32_t x_pos = child % sensor_area.second.x + sensor_area.first.x;
    std::uint32_t y_pos = child / sensor_area.second.y + sensor_area.first.y;

    sub_area = std::make_pair(Point2i(0), Vector2f(1.f));

    return Point2i(x_pos, y_pos);
}

MTS_NAMESPACE_END