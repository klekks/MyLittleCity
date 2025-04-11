#pragma once

#include "../Path/Coordinates.hpp"
#include "Building.hpp"
#include <optional>
#include <unordered_map>
#include <vector>

class CityMap 
{
public:
    void addRoad(int id, Vec2 start, Vec2 end);

    void addBuilding(int id, Vec2 position);

    const std::unordered_map<int, Road>& getRoads() const ;
    const std::unordered_map<int, Building>& getBuildings() const ;
    const std::unordered_map<Vec2, std::vector<std::pair<Vec2, float>>, Vec2Hash>& getGraph() const ;

    std::optional<Vec2> getBuildingEntry(int buildingId) const ;

private:
    std::unordered_map<int, Road> roads;
    std::unordered_map<int, Building> buildings;
    std::unordered_map<Vec2, std::vector<std::pair<Vec2, float>>, Vec2Hash> graph;
    std::unordered_map<int, Vec2> buildingConnections;

    void addIntersection(const Vec2& pt) ;
    int findNearestRoad(const Vec2& position) const ;
    static Vec2 projectToRoad(const Road& road, const Vec2& p) ;
};
