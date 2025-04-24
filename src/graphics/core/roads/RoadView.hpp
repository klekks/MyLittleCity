#pragma once

#include "../../../core/roads/Road.hpp"
#include "../Drawable.hpp"

class RoadView : public Road,
                 public Drawable
{
    void draw() const override;
};