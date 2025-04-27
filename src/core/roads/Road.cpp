#include "Road.hpp"

#include <cmath>


Road::Road(std::shared_ptr<Intersection> begin, std::shared_ptr<Intersection> end) 
: begin_intersection(begin), end_intersection(end)
{}

float Road::length() const 
{
    return begin_intersection.lock()->distance(*end_intersection.lock());
}

OBB Road::getBoundingBox() const 
{
    auto begin = begin_intersection.lock();
    auto end = end_intersection.lock();
    if (!begin || !end) return OBB(Point(0, 0), Point(0, 0), 0); // Возвращаем пустой OBB, если один из указателей истек

    float dx = begin->getX() - end->getX();
    float dy = begin->getY() - end->getY();
    float length = this->length();

    return OBB(
        (begin->coordinates() + end->coordinates()) * 0.5f,
        Point(length / 2.0f, 10.0f),
        atan2f(dy, dx));
}
