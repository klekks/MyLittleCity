#include "Road.hpp"

#include <cmath>


Road::Road(std::shared_ptr<Intersection> begin, std::shared_ptr<Intersection> end) 
: begin_intersection(begin), end_intersection(end)
{
    float dx = begin->getX() - end->getX();
    float dy = begin->getY() - end->getY();

    setBoundingBox({
        (begin->coordinates() + end->coordinates()) * 0.5f,
        {length() / 2.0f, width_},
        atan2f(dy, dx)
    });
}

float Road::length() const 
{
    return begin_intersection.lock()->distance(*end_intersection.lock());
}
