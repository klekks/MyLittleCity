#pragma once 

#include <atomic>

class GameObject
{
    size_t id_ = GameObject::id_counter++;
    static std::atomic<size_t> id_counter;
public:
    size_t id() const { return id_; }
};
