#pragma once

#include "../../core/roads/Road.hpp"
#include "../core/Drawable.hpp"

class RoadView : public Road,
                 public Drawable
{
public:
    RoadView(std::shared_ptr<Intersection>, std::shared_ptr<Intersection>);
    void draw(sf::RenderWindow&) const override;
};