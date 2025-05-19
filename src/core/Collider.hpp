#pragma once

#include <cmath>

constexpr float DOT_DEFAULT_SNAP_RADIUS = 0.01f;

template <typename Type>
struct Point2D
{
    Type x = 0, y = 0;
    Point2D() {}
    Point2D(Type x, Type y) : x(x), y(y) {}

    template <typename OtherType>
    Point2D(const Point2D<OtherType>& other)
    {
        x = other.x;
        y = other.y;
    }

    template <typename OtherType>
    Point2D<Type> operator+(const Point2D<OtherType>& other) const
    {
        return {x + other.x, y + other.y};
    }

    template <typename OtherType>
    Point2D<Type> operator-(const Point2D<OtherType>& other) const
    {
        return {x - other.x, y - other.y};
    }

    template <typename OtherType>
    Point2D<Type> operator*(const Point2D<OtherType>& other) const
    {
        return {x * other.x, y * other.y};
    }

    template <typename OtherType>
    Type dot(const Point2D<OtherType>& other) const 
    {
        return x * other.x + y * other.y;
    }

    template <typename OtherType>
    Type length(const Point2D<OtherType>& other) const 
    {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    Point2D<Type> perpendicular() const 
    {
        return {-y, x};
    }

    Point2D<Type> normalized() const 
    {
        auto len = std::sqrt(x * x + y * y);
        return {x / len, y / len};
    }

    template <typename OtherType>
    bool operator==(const Point2D<OtherType>& other) const 
    {
        return (x - other.x) < 0.001f && (y - other.y) < 0.001f;
    }
};

using Point2Di = Point2D<int>;
using Point2Df = Point2D<float>;

class Collider
{
public:
    virtual bool intersects(const Collider&) const = 0;
    virtual bool intersects(const Collider*) const = 0;
};

class PointCollider;

class BoundingBoxCollider : public Collider 
{
public:
    BoundingBoxCollider(const Point2Df &center, const Point2Df &sizes, float rotation);
    BoundingBoxCollider(float width, const Point2Df &coordinates_from, const Point2Df &coordinates_to);

    virtual bool intersects(const Collider&) const override;
    virtual bool intersects(const Collider*) const override;

    Point2Df get_center() const { return center; }
    Point2Df get_sizes() const { return sizes; }
    float get_rotation() const { return rotation; }

private:
    Point2Df center;
    Point2Df sizes;
    float rotation;

    friend bool intersects(const BoundingBoxCollider* a, const BoundingBoxCollider* b);
    friend bool intersects(const PointCollider* a, const BoundingBoxCollider* b);
};

class PointCollider : public Collider 
{
public:
    PointCollider(const Point2Df&, float = DOT_DEFAULT_SNAP_RADIUS);

    virtual bool intersects(const Collider&) const override;
    virtual bool intersects(const Collider*) const override;

    void set_snap_radius(float radius);
    float get_snap_radius() const;
private:
    Point2Df point;
    float snap_radius;

    friend bool intersects(const PointCollider* a, const PointCollider* b);
    friend bool intersects(const PointCollider* a, const BoundingBoxCollider* b);
};
