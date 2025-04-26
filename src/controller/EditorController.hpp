#pragma once

#include <SFML/Window.hpp>
#include "../driver/World.hpp"
#include <memory>

class EditorController
{
public:
    EditorController(std::shared_ptr<World> w);

    void startRoad(const sf::Vector2i &pos);
    void previewRoad(const sf::Vector2i &pos);
    void finishRoad(const sf::Vector2i &pos);

    void placeBuilding(const sf::Vector2i &pos) ;
    void removeRoad() ;

    bool isDrawing() const ;
    sf::Vector2i getStart() const ;
    sf::Vector2i getCurrent() const ;

private:
    std::shared_ptr<World> world;
    bool drawing = false;
    sf::Vector2i start, current;
};
