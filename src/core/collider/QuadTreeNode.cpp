#include "QuadTreeNode.hpp"
#include <algorithm>

QuadTreeNode::QuadTreeNode(int lvl, const OrientedBoundingBox& b)
    : level(lvl), bounds(b) {}

void QuadTreeNode::clear() {
    objects.clear();
    for (auto& child : children) {
        if (child) {
            child->clear();
            child.reset();
        }
    }
}

void QuadTreeNode::split() {
    Point half_sizes = bounds.getHalfSize();
    Point center = bounds.getCenter();
    float subWidth = half_sizes.getX();
    float subHeight = half_sizes.getY();
    float x = center.getX();
    float y = center.getY();

    children[0] = std::make_unique<QuadTreeNode>(level + 1, OrientedBoundingBox{center - half_sizes * 0.5, half_sizes * 0.5, bounds.getRotation()});
    children[1] = std::make_unique<QuadTreeNode>(level + 1, OrientedBoundingBox{center + Point{+subWidth/2, -subHeight/2}, half_sizes * 0.5, bounds.getRotation()});
    children[2] = std::make_unique<QuadTreeNode>(level + 1, OrientedBoundingBox{center + Point{-subWidth/2, +subHeight/2}, half_sizes * 0.5, bounds.getRotation()});
    children[3] = std::make_unique<QuadTreeNode>(level + 1, OrientedBoundingBox{center + half_sizes * 0.5, half_sizes * 0.5, bounds.getRotation()});
}

int QuadTreeNode::getIndex(const OrientedBoundingBox& box) const {
    bool top = box.center.y < bounds.center.y;
    bool bottom = box.center.y > bounds.center.y;
    bool left = box.center.x < bounds.center.x;
    bool right = box.center.x > bounds.center.x;

    if (top) {
        if (left) return 0; 
        if (right) return 1; 
    }
    if (bottom) {
        if (left) return 2; 
        if (right) return 3; 
    }
    return -1; 
}

void QuadTreeNode::insert(std::shared_ptr<Collider> collider) {
    if (children[0]) {
        int index = getIndex(collider->getBoundingBox());
        if (index != -1) {
            children[index]->insert(collider);
            return;
        }
    }

    objects.push_back(collider);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (!children[0]) {
            split();
        }

        auto it = objects.begin();
        while (it != objects.end()) {
            auto shared = it->lock();
            if (shared) {
                int index = getIndex(shared->getBoundingBox());
                if (index != -1) {
                    children[index]->insert(shared);
                    it = objects.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
}

void QuadTreeNode::retrieve(const OrientedBoundingBox& area, std::vector<std::shared_ptr<Collider>>& outColliders) const {
    int index = getIndex(area);
    if (index != -1 && children[0]) {
        children[index]->retrieve(area, outColliders);
    }

    for (const auto& weak : objects) {
        if (auto shared = weak.lock()) {
            outColliders.push_back(shared);
        }
    }
}
