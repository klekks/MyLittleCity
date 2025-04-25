#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


class Simulation
{
public:
    Simulation();
    void render_loop();
private:
    std::shared_ptr<sf::RenderWindow> window;
};