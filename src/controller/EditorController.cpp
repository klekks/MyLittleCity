#include "EditorController.hpp"

EditorController::EditorController(std::shared_ptr<World> w) : world(w)
{
}


void EditorController::startRoad(const sf::Vector2i &pos)
{
    startIntersectionPreview = std::make_shared<IntersectionView>(Point{(float)pos.x, (float)pos.y});
    endIntersectionPreview = std::make_shared<IntersectionView>(Point{(float)pos.x, (float)pos.y});
    roadPreview = std::make_shared<RoadView>(startIntersectionPreview, endIntersectionPreview);
}


void EditorController::previewRoad(const sf::Vector2i &pos)
{
    endIntersectionPreview = std::make_shared<IntersectionView>(Point{(float)pos.x, (float)pos.y});
    roadPreview = std::make_shared<RoadView>(startIntersectionPreview, endIntersectionPreview);

    if (world->intersects(*roadPreview))
    {
        roadPreview->setColor(sf::Color(255, 0, 0)); // Red color for intersection
    }
}


void EditorController::finishRoad(const sf::Vector2i &pos)
{
    if (roadPreview && startIntersectionPreview && endIntersectionPreview)
    {
        world->addRoad(roadPreview);
        world->addIntersection(startIntersectionPreview);
        world->addIntersection(endIntersectionPreview);
        roadPreview = nullptr;
        startIntersectionPreview = nullptr;
        endIntersectionPreview = nullptr;
    }
}       


void EditorController::placeBuilding(const sf::Vector2i &pos)
{
    world->addBuilding(pos);
}


void EditorController::removeRoad()
{
    world->removeLastRoad();
}


std::vector<std::shared_ptr<Drawable>> EditorController::getPreviews() const
{
    if (!roadPreview || !startIntersectionPreview || !endIntersectionPreview)
        return {};
    return {roadPreview, startIntersectionPreview, endIntersectionPreview};
}