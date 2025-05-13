#include "../view/Renderable.hpp"

class Entity
{
public:
    virtual void update() {}
    virtual bool has_collider() const { return false; }
    virtual ~Entity() = default;
};
