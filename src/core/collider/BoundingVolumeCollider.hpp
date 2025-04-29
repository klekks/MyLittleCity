#pragma once

#include "OrientedBoundingBox.hpp"
#include "Collider.hpp"


class BoundingVolumeCollider : public Collider 
{
private:
    OrientedBoundingBox obb;
public:
    BoundingVolumeCollider(const OrientedBoundingBox& box);
    bool intersects(const Collider& other) const override;
    const OrientedBoundingBox& getOrientedBoundingBox() const; 
};
