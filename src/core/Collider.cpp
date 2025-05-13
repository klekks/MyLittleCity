#include "Collider.hpp"

#include <array>
#include <cmath>
#include <vector>
#include <stdexcept>

BoundingBoxCollider::BoundingBoxCollider(const Point2Df &center, const Point2Df &sizes, float rotation)
    : center(center), sizes(sizes), rotation(rotation)
{}

bool BoundingBoxCollider::intersects(const Collider& other) const
{
    return intersects(&other);
}

std::array<Point2Df, 4> get_corners(const Point2Df &center, const Point2Df &sizes, float rotation)
{
    float dx = sizes.x / 2.0f;
    float dy = sizes.y / 2.0f;
    float cosA = std::cos(rotation);
    float sinA = std::sin(rotation);

    std::array<Point2Df, 4> local = {{
        {-dx, -dy},
        { dx, -dy},
        { dx,  dy},
        {-dx,  dy}
    }};

    std::array<Point2Df, 4> corners;
    for (int i = 0; i < 4; ++i) {
        float x = local[i].x;
        float y = local[i].y;
        corners[i] = {
            center.x + x * cosA - y * sinA,
            center.y + x * sinA + y * cosA
        };
    }
    return corners;
}

void projectOntoAxis(const std::array<Point2Df, 4>& rect, const Point2Df& axis, float& min, float& max) {
    min = max = rect[0].dot(axis);
    for (int i = 1; i < 4; ++i) {
        float projection = rect[i].dot(axis);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
}

bool overlapOnAxis(const std::array<Point2Df, 4>& rect1, const std::array<Point2Df, 4>& rect2, const Point2Df& axis) {
    float min1, max1, min2, max2;
    projectOntoAxis(rect1, axis, min1, max1);
    projectOntoAxis(rect2, axis, min2, max2);
    return !(max1 < min2 || max2 < min1);  
}

bool intersects(const BoundingBoxCollider* a, const BoundingBoxCollider* b)
{
    std::array<Point2Df, 4> rect1 = get_corners(a->center, a->sizes, a->rotation);
    std::array<Point2Df, 4> rect2 = get_corners(b->center, b->sizes, b->rotation);

    std::vector<Point2Df> axes;

    for (int i = 0; i < 2; ++i) {
        Point2Df edge = rect1[(i + 1) % 4] - rect1[i];
        axes.push_back(edge.perpendicular().normalized());
    }

    for (int i = 0; i < 2; ++i) {
        Point2Df edge = rect2[(i + 1) % 4] - rect2[i];
        axes.push_back(edge.perpendicular().normalized());
    }

    for (const Point2Df& axis : axes) {
        if (!overlapOnAxis(rect1, rect2, axis)) {
            return false; 
        }
    }

    return true;
}

bool intersects(const PointCollider* a, const PointCollider* b)
{
    return std::fabs(a->point.x - b->point.x) < DOT_SNAP_RADIUS && std::fabs(a->point.y - b->point.y) < DOT_SNAP_RADIUS;
}

bool intersects(const PointCollider* a, const BoundingBoxCollider* b)
{
    auto bb_a = BoundingBoxCollider(a->point, {DOT_SNAP_RADIUS, DOT_SNAP_RADIUS}, 0);
    return b->intersects(bb_a);
}

bool BoundingBoxCollider::intersects(const Collider* other) const
{
    auto bounding_box_collider = dynamic_cast<const BoundingBoxCollider*>(other);
    if (bounding_box_collider)
    {
        return ::intersects(this, bounding_box_collider);
    }

    auto point_collider = dynamic_cast<const PointCollider*>(other);
    if (point_collider)
    {
        return ::intersects(point_collider, this);
    }

    throw std::runtime_error("Unknown collider type was passed to BoundingBoxCollider::intersects");
    return false;
}

PointCollider::PointCollider(const Point2Df &point)
    : point(point)
{}

bool PointCollider::intersects(const Collider* other) const
{
    auto point_collider = dynamic_cast<const PointCollider*>(other);
    if (point_collider)
    {
        return ::intersects(this, point_collider);
    }

    auto bounding_box_collider = dynamic_cast<const BoundingBoxCollider*>(other);
    if (bounding_box_collider)
    {
        return ::intersects(this, bounding_box_collider);
    }

    throw std::runtime_error("Unknown collider type was passed to BoundingBoxCollider::intersects");
    return false;
}

bool PointCollider::intersects(const Collider& other) const
{
    return intersects(&other);
}
