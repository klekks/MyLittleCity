#include "../core/Entity.hpp"
#include "../core/Collider.hpp"

#include <memory>


class Crossroad : virtual public Entity, virtual public EasyCollider
{
public:
    Crossroad(const Point2Df& coordinates) 
        : coordinates_(coordinates)
    {
        set_collider(std::move(std::make_unique<PointCollider>(coordinates, radius)));
    }

    Point2Df coordinates() const { return coordinates_; }
private:
    Point2Df coordinates_;
    float radius = 15.0f;
};
