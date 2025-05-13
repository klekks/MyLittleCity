#include "../../src/core/Collider.hpp"

#include <gtest/gtest.h>

class UnknownCollider : public Collider
{
public:
    bool intersects(const Collider&) const override { return true; };
    bool intersects(const Collider*) const override { return true; };
};

TEST(BoundingBoxCollider, SelfIntersection)
{
    BoundingBoxCollider collider({30, -40}, {600, 200}, 17);
    ASSERT_TRUE(collider.intersects(collider));
}

TEST(BoundingBoxCollider, IdenticalRectangles) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({0, 0}, {4, 2}, 0);

    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}


TEST(BoundingBoxCollider, PartialOverlap) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({2, 0}, {4, 2}, 0);
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}


TEST(BoundingBoxCollider, OneInsideAnother) {
    BoundingBoxCollider collider1({0, 0}, {4, 4}, 0);
    BoundingBoxCollider collider2({0, 0}, {2, 2}, 0);
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}


TEST(BoundingBoxCollider, TouchingEdges) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({4, 0}, {4, 2}, 0);
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}


TEST(BoundingBoxCollider, TouchingCorners) {
    BoundingBoxCollider collider1({0, 0}, {4, 4}, 0);
    BoundingBoxCollider collider2({4, 4}, {4, 4}, 0);
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}


TEST(BoundingBoxCollider, NoIntersection) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({10, 0}, {4, 2}, 0);
    ASSERT_FALSE(collider1.intersects(collider2));
    ASSERT_FALSE(collider2.intersects(collider1));
}

TEST(BoundingBoxCollider, RotatedOverlap) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({1, 0}, {4, 2}, M_PI_4);
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}

TEST(BoundingBoxCollider, RotatedNoIntersection) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({5, 0}, {4, 2}, M_PI_4);
    ASSERT_FALSE(collider1.intersects(collider2));
    ASSERT_FALSE(collider2.intersects(collider1));
}

TEST(BoundingBoxCollider, AlmostTouchingRotated) {
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2({4.01, 0}, {4, 2}, M_PI_2);
    ASSERT_FALSE(collider1.intersects(collider2));
    ASSERT_FALSE(collider2.intersects(collider1));
}

TEST(BoundingBoxCollider, NoOverlapAfterRotation)
{
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    BoundingBoxCollider collider2_non_rotated({4, 0}, {6, 2}, 0);
    ASSERT_TRUE(collider1.intersects(collider2_non_rotated));
    ASSERT_TRUE(collider2_non_rotated.intersects(collider1));

    BoundingBoxCollider collider2_rotated({4, 0}, {6, 2}, M_PI_2);
    ASSERT_FALSE(collider1.intersects(collider2_rotated));
    ASSERT_FALSE(collider2_rotated.intersects(collider1));
}

TEST(BoundingBoxCollider, AssertIfUnknownCollider)
{
    BoundingBoxCollider collider1({0, 0}, {4, 2}, 0);
    UnknownCollider unknown_collider;

    ASSERT_ANY_THROW(collider1.intersects(unknown_collider));
}

TEST(PointCollider, SelfIntersection)
{
    PointCollider collider({17.f, 42.f});
    ASSERT_TRUE(collider.intersects(collider));
}

TEST(PointCollider, SamePoint)
{
    PointCollider collider1({17.f, 42.f});
    PointCollider collider2({17.f, 42.f});
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}

TEST(PointCollider, AlmostSamePointLessThanSnapRadius)
{
    PointCollider collider1({17.f + 0.99f * DOT_SNAP_RADIUS, 42.f});
    PointCollider collider2({17.f, 42.f});
    ASSERT_TRUE(collider1.intersects(collider2));
    ASSERT_TRUE(collider2.intersects(collider1));
}

TEST(PointCollider, AlmostSamePointMoreThanSnapRadius)
{
    PointCollider collider1({17.f + 1.01f * DOT_SNAP_RADIUS, 42.f});
    PointCollider collider2({17.f, 42.f});
    ASSERT_FALSE(collider1.intersects(collider2));
    ASSERT_FALSE(collider2.intersects(collider1));
}

TEST(PointCollider, DifferentPoints)
{
    PointCollider collider1({130.f, 42.f});
    PointCollider collider2({-54.f, 16.f});
    ASSERT_FALSE(collider1.intersects(collider2));
    ASSERT_FALSE(collider2.intersects(collider1));
}

TEST(PointCollider, AssertIfUnknownCollider)
{
    PointCollider point({0, 0});
    UnknownCollider unknown_collider;
    ASSERT_ANY_THROW(point.intersects(unknown_collider));
}

TEST(PointToBoundingBox, DotInside)
{
    PointCollider point({0, 0});
    BoundingBoxCollider box({0, 0}, {5, 5}, M_PI / 17);
    ASSERT_TRUE(point.intersects(box));
    ASSERT_TRUE(box.intersects(point));
}

TEST(PointToBoundingBox, DotOutside)
{
    PointCollider point({5, 5});
    BoundingBoxCollider box({0, 0}, {5, 5}, 0);
    ASSERT_FALSE(point.intersects(box));
    ASSERT_FALSE(box.intersects(point));
}

TEST(PointToBoundingBox, DotOnTheEdge)
{
    PointCollider point({5, 0});
    BoundingBoxCollider box({0, 0}, {10, 10}, 0);
    ASSERT_TRUE(point.intersects(box));
    ASSERT_TRUE(box.intersects(point));
}

TEST(PointToBoundingBox, DotOnTheCorner)
{
    PointCollider point({5, 5});
    BoundingBoxCollider box({0, 0}, {10, 10}, 0);
    ASSERT_TRUE(point.intersects(box));
    ASSERT_TRUE(box.intersects(point));
}
