#pragma once

#include "Point.hpp"

#include <cinttypes>
#include <array>
#include <memory>

constexpr size_t MAX_ROADS_PER_INTERSECTION = 8;

class Road;

class Intersection
{
    Point coordinates_;
    std::array<std::weak_ptr<Road>, MAX_ROADS_PER_INTERSECTION> connected_roads;

public:
    Intersection(const Point& coordinates);

    bool operator==(const Intersection&) const;
    bool operator<(const Intersection&) const;

    float distance(const Intersection& ) const;
    float distanceX(const Intersection& ) const;
    float distanceY(const Intersection& ) const;

    float getX() const;
    float getY() const;

    const Point& coordinates() const;
};
