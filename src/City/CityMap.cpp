#include "CityMap.hpp"

#include "../Path/Coordinates.hpp"

#include <unordered_map>
#include <vector>

void CityMap::addRoad(int id, Vec2 start, Vec2 end) 
{
    roads[id] = {id, start, end};
    addIntersection(start);
    addIntersection(end);

    float cost = start.distanceTo(end);
    graph[start].emplace_back(end, cost);
    graph[end].emplace_back(start, cost);
}


void CityMap::addBuilding(int id, Vec2 position) 
{
    int nearestRoad = findNearestRoad(position);
    buildings[id] = {id, position, nearestRoad};

    if (nearestRoad != -1) {
        Vec2 proj = projectToRoad(roads[nearestRoad], position);
        graph[position].emplace_back(proj, 0.1f); // маленькая стоимость до дороги
        graph[proj].emplace_back(position, 0.1f);
        buildingConnections[id] = proj;
    }
}


const std::unordered_map<int, Road>& CityMap::getRoads() const 
{ 
    return roads; 
}


const std::unordered_map<int, Building>& CityMap::getBuildings() const 
{ 
    return buildings; 
}


const std::unordered_map<Vec2, std::vector<std::pair<Vec2, float>>, Vec2Hash>& CityMap::getGraph() const 
{
    return graph; 
}


std::optional<Vec2> CityMap::getBuildingEntry(int buildingId) const 
{
    auto it = buildingConnections.find(buildingId);
    if (it != buildingConnections.end()) return it->second;
    return std::nullopt;
}


void CityMap::addIntersection(const Vec2& pt) 
{
    if (graph.find(pt) == graph.end()) {
        graph[pt] = {};
    }
}


int CityMap::findNearestRoad(const Vec2& position) const 
{
    float minDist = std::numeric_limits<float>::max();
    int bestRoad = -1;

    for (const auto& [id, road] : roads) {
        Vec2 proj = projectToRoad(road, position);
        float dist = position.distanceTo(proj);
        if (dist < minDist) {
            minDist = dist;
            bestRoad = id;
        }
    }
    return bestRoad;
}


Vec2 CityMap::projectToRoad(const Road& road, const Vec2& p) 
{
    Vec2 a = road.start;
    Vec2 b = road.end;
    Vec2 ab = {b.x - a.x, b.y - a.y};
    Vec2 ap = {p.x - a.x, p.y - a.y};
    float abLen2 = ab.x * ab.x + ab.y * ab.y;
    float t = std::max(0.f, std::min(1.f, (ap.x * ab.x + ap.y * ab.y) / abLen2));
    return {a.x + ab.x * t, a.y + ab.y * t};
}

