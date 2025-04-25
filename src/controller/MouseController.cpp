#include "MouseController.hpp"

#include <iostream>

class LeftClickCommand : public Command<sf::Event>
{
    void execute(const sf::Event& event) override 
    {
        std::cout << "LMB" << std::endl;
    }
};

class RightClickCommand : public Command<sf::Event>
{
    void execute(const sf::Event& event) override 
    {
        std::cout << "RMB" << std::endl;
    }
};



std::unique_ptr<Command<sf::Event>> MouseController::handle_input(sf::Window& window, const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            return std::make_unique<LeftClickCommand>();
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            return std::make_unique<RightClickCommand>();
        }
    }

    return {};
}