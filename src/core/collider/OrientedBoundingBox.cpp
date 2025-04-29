#include "OrientedBoundingBox.hpp"

#include <cmath>

OrientedBoundingBox::OrientedBoundingBox(const Point& center, const Point& halfSize, float rotation)
        : center(center), halfSize(halfSize), rotation(rotation) {}

std::array<Point, 4> OrientedBoundingBox::getCorners() const
{
    float cosA = cosf(rotation);
    float sinA = sinf(rotation);
    Point right = {cosA, sinA};
    Point up = {-sinA, cosA};

    return {
        center + Point{right.getX() * halfSize.getX() + up.getX() * halfSize.getY(), right.getY() * halfSize.getX() + up.getY() * halfSize.getY()},
        center + Point{right.getX() * halfSize.getX() - up.getX() * halfSize.getY(), right.getY() * halfSize.getX() - up.getY() * halfSize.getY()},
        center + Point{-right.getX() * halfSize.getX() - up.getX() * halfSize.getY(), -right.getY() * halfSize.getX() - up.getY() * halfSize.getY()},
        center + Point{-right.getX() * halfSize.getX() + up.getX() * halfSize.getY(), -right.getY() * halfSize.getX() + up.getY() * halfSize.getY()}};
}

bool OrientedBoundingBox::intersects(const OrientedBoundingBox &other) const
{
    return obbIntersect(*this, other);
}

void projectOntoAxis(const std::array<Point, 4> &corners, const Point &axis, float &min, float &max)
{
    min = max = corners[0].dot(axis);
    for (int i = 1; i < 4; ++i)
    {
        float projection = corners[i].dot(axis);
        min = std::fmin(min, projection);
        max = std::fmax(max, projection);
    }
}

bool obbIntersect(const OrientedBoundingBox &a, const OrientedBoundingBox &b)
{
    auto cornersA = a.getCorners();
    auto cornersB = b.getCorners();

    Point axes[4] = {
        (cornersA[1] - cornersA[0]),
        (cornersA[3] - cornersA[0]),
        (cornersB[1] - cornersB[0]),
        (cornersB[3] - cornersB[0])};

    for (auto &axis : axes)
    {
        float length = sqrtf(axis.getX() * axis.getX() + axis.getY() * axis.getY());
        axis = {axis.getX() / length, axis.getY() / length};

        float minA, maxA, minB, maxB;
        projectOntoAxis(cornersA, axis, minA, maxA);
        projectOntoAxis(cornersB, axis, minB, maxB);

        if (maxA < minB || maxB < minA)
        {
            return false;
        }
    }
    return true; 
}
