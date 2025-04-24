#include "Point.hpp"

#include <cmath>

constexpr float POINT_SNAP_DISTANCE = 0.1;

Point::Point(float x, float y) : x(x), y(y) {}

bool Point::operator<(const Point& other) const
{
    if (x < other.x) return true;
    if (x == other.x) return y < other.y;
    return false;
}

bool Point::operator==(const Point& other) const
{
    return distance(other) < POINT_SNAP_DISTANCE;
}

float Point::distance(const Point& other) const
{
    float dx = (other.x - x);
    float dy = (other.y - y);
    return sqrtf(dx * dx + dy * dy);
}
