#pragma once

#include <SFML/Graphics.hpp>
#include "../core/Entity.hpp"

class Renderable : virtual public Entity
{
public:
    virtual void render(sf::RenderTarget &target) const = 0;
    virtual bool is_renderable() const final override { return true; }
    virtual ~Renderable() = default;
};
