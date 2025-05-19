#include <memory>

#include "../core/Entity.hpp"
#include "Crossroad.hpp"
#include "../core/Collider.hpp"



class Road : virtual public Entity, virtual public EasyCollider
{
public:
    Road(std::shared_ptr<Crossroad> from, std::shared_ptr<Crossroad> to, float width = 15.f)
        : from(from), to(to), width(width)
    {
        set_collider(std::move(std::make_unique<BoundingBoxCollider>(width, from, to)));
    }
private:
    std::shared_ptr<Crossroad> from, to;
    float width;
};
