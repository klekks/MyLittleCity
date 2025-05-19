#pragma once
#include "Collider.hpp"
#include <memory>

class Entity
{
public:
    virtual void update() {}
    virtual bool has_collider() const { return false; }
    virtual const Collider* get_collider() const { return nullptr; }
    virtual bool is_renderable() const { return false; }
    virtual ~Entity() = default;
};

class EasyCollider : virtual public Entity
{
public:
    bool has_collider() const final override  { return collider.operator bool(); }
    const Collider* get_collider() const final override { return collider.get(); }
private:
    std::unique_ptr<Collider> collider;
protected:
    void set_collider(std::unique_ptr<Collider> collider_) { collider = std::move(collider_); }
};
