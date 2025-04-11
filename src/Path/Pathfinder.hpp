#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "Coordinates.hpp"


class Pathfinder 
{
public:
    static std::vector<Vec2> findPath(const std::unordered_map<Vec2, std::vector<std::pair<Vec2, float>>, Vec2Hash>& graph,Vec2 start, Vec2 goal);
private:
    static std::unordered_map<Vec2, float, Vec2Hash> fScore;
    static float heuristic(const Vec2& a, const Vec2& b);
    static std::vector<Vec2> reconstructPath(std::unordered_map<Vec2, Vec2, Vec2Hash>& cameFrom, Vec2 current);
};
