#include "Simulation.hpp"

#include <iostream>

#include "../driver/World.hpp"
#include "../graphics/Renderer.hpp"
#include "../controller/InputBinding.hpp"    
#include "../controller/InputHandler.hpp"    
#include "../controller/EditorController.hpp"    
#include "../controller/commands/MouseCommands.hpp"    
#include "Camera.hpp"

Simulation::Simulation()
{
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "MyLittleCity");
}

void Simulation::render_loop()
{
    std::shared_ptr<World> world = std::make_shared<World>();
    auto camera = std::make_shared<Camera>();
    Renderer renderer(world, camera);
    EditorController controller(world);
    InputContext inputContext;
    InputHandler inputHandler;

    inputHandler.bind({InputActionType::MouseDown, sf::Mouse::Left}, std::make_shared<MouseDownCommand>(&controller, &inputContext));
    inputHandler.bind({InputActionType::Hold, sf::Mouse::Left}, std::make_shared<HoldCommand>(&controller, &inputContext));
    inputHandler.bind({InputActionType::MouseUp, sf::Mouse::Left}, std::make_shared<MouseUpCommand>(&controller, &inputContext));

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1)
                    inputContext.setMode(EditorMode::DrawRoad);
                else if (event.key.code == sf::Keyboard::Num2)
                    inputContext.setMode(EditorMode::PlaceBuilding);
                else if (event.key.code == sf::Keyboard::Num3)
                    inputContext.setMode(EditorMode::RemoveRoad);
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    camera->zoomIn();
                else
                    camera->zoomOut();
            }
            

            inputHandler.handleInput(event, *window);
        }

        inputHandler.handleHold(*window);

        window->clear(sf::Color::Green);
        renderer.render(*window, controller.getPreviews());
        window->display();
    }
    
}
