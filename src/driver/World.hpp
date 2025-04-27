#pragma once

#include <SFML/Window.hpp>
#include <vector>

#include "../graphics/roads/RoadView.hpp"
#include "../graphics/roads/IntersectionView.hpp"

class World
{
public:
    void addRoad(const sf::Vector2i &a, const sf::Vector2i &b);
    void addRoad(std::shared_ptr<RoadView> road);
    void addIntersection(std::shared_ptr<IntersectionView> intersection);

    void addBuilding(const sf::Vector2i &pos);
    void removeLastRoad();

    const std::vector<std::shared_ptr<RoadView>> &getRoads() const;
    const std::vector<sf::Vector2i> &getBuildings() const;

    bool intersects(const RoadView &road) const;

private:
    std::vector<std::shared_ptr<RoadView>> roads;
    std::vector<std::shared_ptr<IntersectionView>> intersections;
    std::vector<sf::Vector2i> buildings;
};
