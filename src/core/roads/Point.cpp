#include "Point.hpp"

#include <cmath>

constexpr float POINT_SNAP_DISTANCE = 0.1;

Point::Point(float x, float y) : x(x), y(y) {}

Point::Point(const std::initializer_list<float> &list) : x(*list.begin()), y(*(list.begin() + 1)) {}

bool Point::operator<(const Point &other) const
{
    if (x < other.x)
        return true;
    if (x == other.x)
        return y < other.y;
    return false;
}

bool Point::operator==(const Point &other) const
{
    return distance(other) < POINT_SNAP_DISTANCE;
}

float Point::distance(const Point &other) const
{
    float dx = distanceX(other);
    float dy = distanceY(other);
    return sqrtf(dx * dx + dy * dy);
}

float Point::distanceX(const Point &other) const
{
    return fabsf(other.x - x);
}

float Point::distanceY(const Point &other) const
{
    return fabsf(other.y - y);
}

Point Point::operator+(const Point &other) const
{
    return Point{x + other.x, y + other.y};
}

Point Point::operator-(const Point &other) const
{
    return Point{x - other.x, y - other.y};
}

Point Point::operator*(float scale) const
{
    return Point{x * scale, y * scale};
}

sf::Vector2f Point::to_sfml_vector2f() const
{
    return {x, y};
}

float Point::getX() const
{
    return x;
}

float Point::getY() const
{
    return y;
}
