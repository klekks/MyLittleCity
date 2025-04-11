
#include "Simulator.hpp"

#include "../Path/Pathfinder.hpp"

#include <iostream>


Simulator::Simulator(CityMap& city) : cityMap(city) {}

void Simulator::addCar(int id, int fromBuilding, int toBuilding) {
    auto fromOpt = cityMap.getBuildingEntry(fromBuilding);
    auto toOpt = cityMap.getBuildingEntry(toBuilding);
    if (!fromOpt || !toOpt) 
        return;

    Vec2 from = *fromOpt;
    Vec2 to = *toOpt;

    Car car{id, from, {}};
    car.path = Pathfinder::findPath(cityMap.getGraph(), from, to);
    cars.push_back(car);
}


void Simulator::tick() 
{
    for (auto& car : cars) 
    {
        if (!car.hasArrived()) 
        {
            car.moveForward();
            std::cout << "Car " << car.id << " moved to (" << car.currentPosition.x << ", " << car.currentPosition.y << ")\n";
        } 
        else 
        {
            std::cout << "Car " << car.id << " has arrived at destination.\n";
        }
    }
}
    