#pragma once 

#include <SFML/Graphics.hpp>

class Drawable
{
public:
    virtual void draw(sf::RenderWindow&) const = 0;
};