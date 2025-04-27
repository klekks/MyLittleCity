#include "World.hpp"

void World::addRoad(const sf::Vector2i &a, const sf::Vector2i &b)
{
    auto i1 = std::make_shared<IntersectionView>(Point({(float)a.x, (float)a.y}));
    auto i2 = std::make_shared<IntersectionView>(Point({(float)b.x, (float)b.y}));
    intersections.push_back(i1);
    intersections.push_back(i2);
    roads.emplace_back(std::make_shared<RoadView>(i1, i2));
}

void World::addBuilding(const sf::Vector2i &pos)
{
    buildings.push_back(pos);
}

void World::addRoad(std::shared_ptr<RoadView> road)
{
    roads.push_back(road);
}

void World::addIntersection(std::shared_ptr<IntersectionView> intersection)
{
    intersections.push_back(intersection);
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


bool World::intersects(const RoadView &road) const
{
    auto bb = road.getBoundingBox();
    for (const auto &r : roads)
    {
        if (r->getBoundingBox().intersects(bb))
            return true;
    }
    return false;
}
