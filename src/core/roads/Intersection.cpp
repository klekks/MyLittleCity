#include "Intersection.hpp"

bool Intersection::operator==(const Intersection& other) const
{
    return coordinates == other.coordinates;
}


bool Intersection::operator<(const Intersection& other) const
{
    return coordinates < other.coordinates;
}


float Intersection::distance(const Intersection& other) const
{
    return coordinates.distance(other.coordinates);
}