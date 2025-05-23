#include <SFML/Graphics.hpp>
#include "controller/InputController.hpp"
#include "core/Entity.hpp"
#include "view/game_objects/Crossroad.hpp"
// #include "game_objects/Road.hpp"
#include "view/game_objects/Road.hpp"
#include <iostream>
#include <vector>
#include <memory>

std::vector<std::shared_ptr<Entity>> objects;

class MyCommand : public InputCommand
{
public:
    void execute(const InputEvent &event, const InputContext &context) override
    {
        if (event.type == InputEventType::MouseDragged)
        {
            if (!in_drug)
            {
                begin_intersection = std::make_shared<CrossroadView>(Point2Df{context.mousePosition.x, context.mousePosition.y});
            }
            else
            {
                end_intersection = std::make_shared<CrossroadView>(Point2Df{context.mousePosition.x, context.mousePosition.y});
                road = std::make_shared<RoadView>(begin_intersection, end_intersection);
            }
            in_drug = true;
        }
        if (event.button == sf::Mouse::Left && event.type == InputEventType::MouseReleased && in_drug)
        {
            in_drug = false;
            end_intersection = std::make_shared<CrossroadView>(Point2Df{context.mousePosition.x, context.mousePosition.y});
            road = std::make_shared<RoadView>(begin_intersection, end_intersection);

            objects.push_back(begin_intersection);
            objects.push_back(end_intersection);
            objects.push_back(road);

            begin_intersection = nullptr;
            end_intersection = nullptr;
            road = nullptr;
        }
    }
private:
    bool in_drug = false;
    std::shared_ptr<Crossroad> begin_intersection;
    std::shared_ptr<Crossroad> end_intersection;
    std::shared_ptr<Road> road;
} command;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Application");
    InputController inputController;

    inputController.bindDrag(sf::Mouse::Left, &command);
    inputController.bind(InputEventType::MouseReleased, &command);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            inputController.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (const auto& obj: objects)
        {
            if (obj->is_renderable())
            {
                auto target = dynamic_cast<Renderable*>(obj.get());
                target->render(window);
            }
        }

        window.display();
    }

    return 0;
}