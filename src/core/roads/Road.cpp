#include "Road.hpp"


Road::Road(std::shared_ptr<Intersection> begin, std::shared_ptr<Intersection> end) 
: begin_intersection(begin), end_intersection(end)
{}

float Road::length() const 
{
    return begin_intersection.lock()->distance(*end_intersection.lock());
}
