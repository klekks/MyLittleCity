#include "../core/Entity.hpp"
#include "../core/Collider.hpp"

#include <memory>


class Crossroad : virtual public Entity
{
public:
    Crossroad(const Point2Df& coordinates) 
        : coordinates(coordinates),
          collider(std::make_unique<PointCollider>(coordinates, radius))
    {}

    bool has_collider() const override { return collider.operator bool(); }
    const Collider* get_collider() { return collider.get(); }
private:
    Point2Df coordinates;
    float radius = 15.0f;
    std::unique_ptr<Collider> collider;
};
