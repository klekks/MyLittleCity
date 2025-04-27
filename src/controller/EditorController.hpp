#pragma once

#include <SFML/Window.hpp>
#include "../driver/World.hpp"
#include "../graphics/roads/IntersectionView.hpp"

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

    std::vector<std::shared_ptr<Drawable>> getPreviews() const;
private:
    std::shared_ptr<World> world;
    
    std::shared_ptr<RoadView> roadPreview;
    std::shared_ptr<IntersectionView> startIntersectionPreview, endIntersectionPreview;
};
