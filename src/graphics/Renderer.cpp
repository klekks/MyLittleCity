#include "Renderer.hpp"
#include <memory>

Renderer::Renderer(std::shared_ptr<World> world, std::shared_ptr<Camera> camera) : world(world), camera(camera) {}

void Renderer::render(sf::RenderWindow &window, const std::vector<std::shared_ptr<Drawable>> &previews)
{
    sf::View view = window.getDefaultView();
    view.zoom(1.0f / camera->getZoom());
    window.setView(view);

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

    if (!previews.empty())
    {
        for (const auto &preview : previews)
        {
            preview->draw(window);
        }
    }
}