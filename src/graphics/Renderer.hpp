#pragma once

#include "../driver/World.hpp"
#include <SFML/Graphics.hpp>
#include "../driver/Camera.hpp"
#include "../graphics/core/Drawable.hpp"

#include <memory>
#include <vector>

class Renderer
{
public:
    Renderer(std::shared_ptr<World> world, std::shared_ptr<Camera> camera);
    void render(sf::RenderWindow &window, const std::vector<std::shared_ptr<Drawable>> &previews);
private:
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;
};
