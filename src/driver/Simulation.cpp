#include "Simulation.hpp"

#include <iostream>

#include "../controller/DelegateController.hpp"

Simulation::Simulation()
{
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(200, 200), "MyLittleCity");
}

void Simulation::render_loop()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    auto controller = make_delegate_controller();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            auto command = controller->handle_input(*window, event);
            if (command)
                command->execute(event);
            else
                std::cout << "Unknown command" << std::endl;
        }

        window->clear();
        window->draw(shape);
        window->display();
    }
}
