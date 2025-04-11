#include "Coordinates.hpp"

#include <cstdint>
#include <cmath>
#include <functional>


float Vec2::distanceTo(const Vec2& other) const 
{
    return std::hypot(x - other.x, y - other.y);
}

bool Vec2::operator==(const Vec2& other) const 
{
    return std::fabs(x - other.x) < 0.001f && std::fabs(y - other.y) < 0.001f;
}

std::size_t Vec2Hash::operator()(const Vec2& v) const 
{
    return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
}

float Road::length() const 
{
    return start.distanceTo(end);
}
