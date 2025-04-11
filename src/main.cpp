
#include <iostream>

#include "Path/Coordinates.hpp"
#include "Transport/Car.hpp"
#include "City/CityMap.hpp"
#include "Simulator/Simulator.hpp"

    
int main() 
{
    CityMap city;
    // дороги как сегменты
    city.addRoad(1, {0, 0}, {5, 0});
    city.addRoad(2, {5, 0}, {5, 5});
    city.addRoad(3, {0, 0}, {0, 5});

    // здания просто точки
    city.addBuilding(1, {0.1f, 0.2f});
    city.addBuilding(2, {5.2f, 5.1f});

    Simulator sim(city);
    sim.addCar(101, 1, 2);

    for (int i = 0; i < 10; ++i) 
    {
        std::cout << "--- Tick " << i << " ---\n";
        sim.tick();
    }

    return 0;
}
    

