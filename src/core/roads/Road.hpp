#pragma once

#include "Intersection.hpp"

class Road
{
private:
    Intersection &begin_intersection, end_intersection;

public:
    Road(Intersection& begin, Intersection& end);

    float length() const;


};
