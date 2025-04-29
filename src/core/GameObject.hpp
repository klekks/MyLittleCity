#pragma once 

#include <atomic>
#include <memory>
#include <cstddef>

#include "collider/Collider.hpp"

class GameObject
{
private:
    size_t id_ = GameObject::id_counter++;
    static std::atomic<size_t> id_counter;

    std::unique_ptr<Collider> collider;

public:
    size_t id() const;

    Collider* getCollider() const;
    void setCollider(std::unique_ptr<Collider>&& collider);
};
