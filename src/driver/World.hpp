#pragma once

#include <SFML/Window.hpp>
#include <vector>

#include "../graphics/roads/RoadView.hpp"

class World
{
public:
    void addRoad(const sf::Vector2i &a, const sf::Vector2i &b);
    void addBuilding(const sf::Vector2i &pos);
    void removeLastRoad();

    const std::vector<std::shared_ptr<RoadView>> &getRoads() const;
    const std::vector<sf::Vector2i> &getBuildings() const;

private:
    std::vector<std::shared_ptr<RoadView>> roads;
    std::vector<std::shared_ptr<Intersection>> intersections;
    std::vector<sf::Vector2i> buildings;
};
