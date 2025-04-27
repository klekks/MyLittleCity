#pragma once

#include "../core/Drawable.hpp"
#include "../../core/roads/Intersection.hpp"


class IntersectionView : public Drawable, public Intersection
{
public: 
    IntersectionView(const Point&);
    void draw(sf::RenderWindow &window) const override;
};
