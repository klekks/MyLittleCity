#include "BoundingVolumeCollider.hpp"

BoundingVolumeCollider::BoundingVolumeCollider(const OrientedBoundingBox& box)
    : obb(box)
{}

bool BoundingVolumeCollider::intersects(const Collider& collider) const
{
    const BoundingVolumeCollider *other = dynamic_cast<const BoundingVolumeCollider*>(&collider);
    if (other)
    {
        return other->getOrientedBoundingBox().intersects(obb);
    }
    else 
    {
        // Unknown Collider type
    }
    return false;
}

const OrientedBoundingBox& BoundingVolumeCollider::getOrientedBoundingBox() const
{
    return obb;
}

std::shared_ptr<GameObject> Collider::getOwner() const 
{
    return owner.lock(); // or getOwnerAs<GameObject>()
}

void Collider::setOwner(std::shared_ptr<GameObject> owner_)
{
    owner = owner_;
}