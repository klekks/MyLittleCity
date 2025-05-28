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

class Hoverable : virtual public Renderable 
{
public:
    void set_hover_on() { hover_status_ = true; }
    void set_hover_off() { hover_status_ = false; }
    bool get_hover_status() const { return hover_status_; }
private:
    bool hover_status_ = false;
};
