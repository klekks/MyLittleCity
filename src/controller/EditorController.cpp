#include "EditorController.hpp"


EditorController::EditorController(std::shared_ptr<World> w) : world(w)
{
}


void EditorController::startRoad(const sf::Vector2i &pos)
{
    drawing = true;
    start = current = pos;
}


void EditorController::previewRoad(const sf::Vector2i &pos)
{
    if (drawing)
        current = pos;
}


void EditorController::finishRoad(const sf::Vector2i &pos)
{
    if (drawing)
    {
        world->addRoad(start, pos);
        drawing = false;
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


bool EditorController::isDrawing() const
{
    return drawing;
}


sf::Vector2i EditorController::getStart() const
{
    return start;
}


sf::Vector2i EditorController::getCurrent() const
{
    return current;
}
