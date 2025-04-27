#pragma once

#include "Intersection.hpp"
#include "../GameObject.hpp"
#include "../OrientedBoundingBox.hpp"

class Road : public GameObject
{
protected:
    std::weak_ptr<Intersection> begin_intersection, end_intersection;
    float width_ = 20.0f; 
public:
    Road(std::shared_ptr<Intersection> begin, std::shared_ptr<Intersection> end);

    float length() const;
    OBB getBoundingBox() const;
};
