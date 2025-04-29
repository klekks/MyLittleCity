#include "Road.hpp"

#include <cmath>

#include "../collider/BoundingVolumeCollider.hpp"
#include "../collider/OrientedBoundingBox.hpp"


Road::Road(std::shared_ptr<Intersection> begin, std::shared_ptr<Intersection> end) 
: begin_intersection(begin), end_intersection(end)
{
    float dx = begin->getX() - end->getX();
    float dy = begin->getY() - end->getY();

    OrientedBoundingBox box{
        (begin->coordinates() + end->coordinates()) * 0.5f,
        {length() / 2.0f, width_},
        atan2f(dy, dx)
    };

    auto collider = std::make_unique<BoundingVolumeCollider>(box);
    setCollider(std::move(collider));
}

float Road::length() const 
{
    return begin_intersection.lock()->distance(*end_intersection.lock());
}
