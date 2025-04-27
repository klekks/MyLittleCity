#include <array>
#include "roads/Point.hpp"

struct OBB
{
private:
    Point center;
    Point halfSize; 
    float rotation; 

public:
    OBB(const Point& center, const Point& halfSize, float rotation);
    std::array<Point, 4> getCorners() const;
    bool intersects(const OBB& other) const;
};

bool obbIntersect(const OBB& a, const OBB& b);

