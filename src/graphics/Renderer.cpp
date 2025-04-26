#include "Renderer.hpp"

Renderer::Renderer(std::shared_ptr<World> world) : world(world) {}

void Renderer::render(sf::RenderWindow &window, bool preview, sf::Vector2i start, sf::Vector2i current)
{
    for (auto &road : world->getRoads())
    {
        road->draw(window);
    }

    for (auto &pos : world->getBuildings())
    {
        sf::CircleShape building(5);
        building.setFillColor(sf::Color::Green);
        building.setPosition(pos.x - 5, pos.y - 5);
        window.draw(building);
    }

    if (preview)
    {
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(start)), sf::Vertex(sf::Vector2f(current))};
        window.draw(line, 2, sf::Lines);
    }
}