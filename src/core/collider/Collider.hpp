#pragma once

#include <memory>

class GameObject;

class Collider
{
private:
    std::weak_ptr<GameObject> owner;

public: 
    virtual ~Collider() = default;
    virtual bool intersects(const Collider& other) const = 0;

    std::shared_ptr<GameObject> getOwner() const;
    void setOwner(std::shared_ptr<GameObject> owner);
};
