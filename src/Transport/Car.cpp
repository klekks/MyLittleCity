#include "Car.hpp"


bool Car::hasArrived() const 
{
    return pathIndex >= path.size();
}

void Car::moveForward() 
{
    if (!hasArrived()) 
    {
        currentPosition = path[pathIndex];
        ++pathIndex;
    }
}
