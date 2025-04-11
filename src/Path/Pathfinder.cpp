#include "Pathfinder.hpp"

#include <algorithm>

std::vector<Vec2> Pathfinder::findPath(
    const std::unordered_map<Vec2, std::vector<std::pair<Vec2, float>>, Vec2Hash>& graph,
    Vec2 start, Vec2 goal)
{
    std::unordered_map<Vec2, float, Vec2Hash> gScore;
    std::unordered_map<Vec2, Vec2, Vec2Hash> cameFrom;
    std::unordered_set<Vec2, Vec2Hash> visited;
    auto cmp = [&](const Vec2& a, const Vec2& b) {
        return fScore[a] > fScore[b];
    };

    std::priority_queue<Vec2, std::vector<Vec2>, decltype(cmp)> openSet(cmp);

    gScore[start] = 0.0f;
    fScore[start] = heuristic(start, goal);
    openSet.push(start);

    while (!openSet.empty()) 
    {
        Vec2 current = openSet.top();
        openSet.pop();

        if (current == goal)
            return reconstructPath(cameFrom, current);

        visited.insert(current);

        for (auto& [neighbor, cost] : graph.at(current)) 
        {
            float tentative_g = gScore[current] + cost;
            if (gScore.find(neighbor) == gScore.end() || tentative_g < gScore[neighbor]) 
            {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_g;
                fScore[neighbor] = tentative_g + heuristic(neighbor, goal);
                openSet.push(neighbor);
            }
        }
    }

    return {};
}


std::unordered_map<Vec2, float, Vec2Hash> Pathfinder::fScore;

float Pathfinder::heuristic(const Vec2& a, const Vec2& b) 
{
    return a.distanceTo(b);
}

std::vector<Vec2> Pathfinder::reconstructPath(
    std::unordered_map<Vec2, Vec2, Vec2Hash>& cameFrom, Vec2 current)
{
    std::vector<Vec2> path = {current};
    while (cameFrom.find(current) != cameFrom.end()) 
    {
        current = cameFrom[current];
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
