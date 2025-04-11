#pragma once

#include "../City/CityMap.hpp"
#include "../Transport/Car.hpp"

#include <vector>

class Simulator {
    public:
        Simulator(CityMap& city);
        void addCar(int id, int fromBuilding, int toBuilding);
        void tick();
    private:
        CityMap& cityMap;
        std::vector<Car> cars;
};
