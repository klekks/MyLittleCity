#pragma once 

#include <vector>
#include "Collider.hpp"


class CollisionWorld
{
private:
    std::vector<Collider*> colliders;

public:
    void addCollider(Collider * collider);
    void removeCollider(Collider * collider);

    std::vector<Collider*> detectCollisions(const Collider *);
};
