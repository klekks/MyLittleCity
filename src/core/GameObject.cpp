#include "GameObject.hpp"

std::atomic<size_t> GameObject::id_counter = 0;

size_t GameObject::id() const { return id_; }

Collider* GameObject::getCollider() const {return collider.get(); }

void GameObject::setCollider(std::unique_ptr<Collider>&& new_collider)
{
    collider.swap(new_collider);
    new_collider.release();
}
