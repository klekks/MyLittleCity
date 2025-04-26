#include "Intersection.hpp"

Intersection::Intersection(const Point &coordinates)
    : coordinates_(coordinates)
{}


bool Intersection::operator==(const Intersection& other) const
{
    return coordinates_ == other.coordinates_;
}


bool Intersection::operator<(const Intersection& other) const
{
    return coordinates_ < other.coordinates_;
}


float Intersection::distance(const Intersection& other) const
{
    return coordinates_.distance(other.coordinates_);
}

float Intersection::distanceX(const Intersection& other) const
{
    return coordinates_.distanceX(other.coordinates_);
}

float Intersection::distanceY(const Intersection& other) const
{
    return coordinates_.distanceY(other.coordinates_);
}

const Point& Intersection::coordinates() const
{
    return coordinates_;
}

float Intersection::getX() const
{
    return coordinates_.getX();
}

float Intersection::getY() const
{
    return coordinates_.getY();
}

