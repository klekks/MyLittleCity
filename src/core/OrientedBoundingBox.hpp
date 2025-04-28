#include <array>
#include "roads/Point.hpp"

struct OBB
{
private:
    Point center = {0.f, 0.f};
    Point halfSize = {0.f, 0.f}; 
    float rotation = 0.f; 

public:
    OBB(const Point& center, const Point& halfSize, float rotation);
    OBB() = default;
    std::array<Point, 4> getCorners() const;
    bool intersects(const OBB& other) const;
};

bool obbIntersect(const OBB& a, const OBB& b);

class ObjectColider
{
public:
    const OBB& getBoundingBox() const;
protected:
    void setBoundingBox(const OBB&);
    OBB bounding_box_;
};
