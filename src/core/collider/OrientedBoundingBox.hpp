#include <array>
#include "../roads/Point.hpp"

struct OrientedBoundingBox
{
private:
    Point center = {0.f, 0.f};
    Point halfSize = {0.f, 0.f}; 
    float rotation = 0.f; 

public:
    OrientedBoundingBox(const Point& center, const Point& halfSize, float rotation);
    OrientedBoundingBox() = default;
    std::array<Point, 4> getCorners() const;
    bool intersects(const OrientedBoundingBox& other) const;
};

bool obbIntersect(const OrientedBoundingBox& a, const OrientedBoundingBox& b);

