#pragma once

#include "../driver/World.hpp"
#include <SFML/Graphics.hpp>

#include <memory>

class Renderer
{
public:
    Renderer(std::shared_ptr<World> world);
    void render(sf::RenderWindow &window, bool preview, sf::Vector2i start, sf::Vector2i current);
private:
    std::shared_ptr<World> world;
};