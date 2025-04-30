#pragma once

#include <vector>
#include <memory>
#include "Collider.hpp"
#include "OrientedBoundingBox.hpp"

class QuadTreeNode {
private:
    static constexpr int MAX_OBJECTS = 4;
    static constexpr int MAX_LEVELS = 5;

    int level;
    OrientedBoundingBox bounds;
    std::vector<std::weak_ptr<Collider>> objects;
    std::unique_ptr<QuadTreeNode> children[4];

public:
    QuadTreeNode(int level, const OrientedBoundingBox& bounds);

    void clear();

    void insert(std::shared_ptr<Collider> collider);

    void retrieve(const OrientedBoundingBox& area, std::vector<std::shared_ptr<Collider>>& outColliders) const;

private:
    void split();
    int getIndex(const OrientedBoundingBox& box) const;
};
