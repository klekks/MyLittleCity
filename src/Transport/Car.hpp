#pragma once

#include "../Path/Coordinates.hpp"
#include <vector>


struct Car 
{
    int id;
    Vec2 currentPosition;
    std::vector<Vec2> path;
    size_t pathIndex = 0;

    bool hasArrived() const;

    void moveForward() ;
};
