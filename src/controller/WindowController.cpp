#include "WindowController.hpp"
#include <iostream>

class CloseWindowCommand : public Command<sf::Event>
{
public:
    CloseWindowCommand(sf::Window& window) : window(window) {}

    void execute(const sf::Event& event) override 
    {
        std::cout << "Close window" << std::endl;
        window.close();
    }
private:
    sf::Window& window;
};



std::unique_ptr<Command<sf::Event>> WindowController::handle_input(sf::Window& window, const sf::Event& event)
{
    if (event.type == sf::Event::Closed)
    {
        return std::make_unique<CloseWindowCommand>(window);
    }

    return {};
}