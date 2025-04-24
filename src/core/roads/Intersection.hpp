#include "Point.hpp"
#include "Road.hpp"

#include <cinttypes>
#include <array>
#include <memory>

constexpr size_t MAX_ROADS_PER_INTERSECTION = 8;

class Intersection
{
    Point coordinates;
    std::array<std::shared_ptr<Road>, MAX_ROADS_PER_INTERSECTION> connected_roads;

public:
    bool operator==(const Intersection&) const;
    bool operator<(const Intersection&) const;
    float distance(const Intersection& ) const;
};
