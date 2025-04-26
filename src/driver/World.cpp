#include "World.hpp"

void World::addRoad(const sf::Vector2i &a, const sf::Vector2i &b)
{
    auto i1 = intersections.emplace_back(std::make_shared<Intersection>(Point{(float)a.x, (float)a.y}));
    auto i2 = intersections.emplace_back(std::make_shared<Intersection>(Point{(float)b.x, (float)b.y}));
    roads.emplace_back(std::make_shared<RoadView>(i1, i2));
}

void World::addBuilding(const sf::Vector2i &pos)
{
    buildings.push_back(pos);
}

void World::removeLastRoad()
{
    if (!roads.empty())
        roads.pop_back();
}

const std::vector<std::shared_ptr<RoadView>> &World::getRoads() const
{
    return roads;
}

const std::vector<sf::Vector2i> &World::getBuildings() const
{
    return buildings;
}
