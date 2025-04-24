#include "Road.hpp"


Road::Road(Intersection& begin, Intersection& end) 
: begin_intersection(begin), end_intersection(end)
{}

float Road::length() const 
{
    return begin_intersection.distance(end_intersection);
}
