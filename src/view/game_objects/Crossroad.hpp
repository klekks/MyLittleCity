#pragma once 
#include "../Renderable.hpp"
#include "../../game_objects/Crossroad.hpp"

class CrossroadView : virtual public Crossroad, virtual public Hoverable
{
public:
    CrossroadView(const Point2Df& coordinates) : Crossroad(coordinates) {}

    void render(sf::RenderTarget& target) const override
    {
        sf::CircleShape shape(radius);
        shape.setFillColor(fill_color);
        shape.setOutlineColor(outline_color);
        shape.setOutlineThickness(1.f);
        shape.setPosition({coordinates().x - radius, coordinates().y - radius});

        if (get_hover_status())
            shape.setOutlineColor(hover_outline_color);

        target.draw(shape);
    }
private:
    sf::Color fill_color = sf::Color(68, 68, 68), outline_color = sf::Color::White, hover_outline_color = sf::Color::Red;
};
