#include "../view/Renderable.hpp"

class Entity
{
public:
    virtual void update() {}
    virtual bool has_collisions() const { return false; }
    virtual bool is_renderable() const { return false; }
    virtual ~Entity() = default;
};




