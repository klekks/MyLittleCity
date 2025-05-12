#include <SFML/Graphics.hpp>
#include "controller/InputController.hpp"
#include <iostream>

class MyCommand : public InputCommand
{
public:
    void execute(const InputEvent &event, const InputContext &context) override
    {
        
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Application");
    InputController inputController;

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
        window.display();
    }

    return 0;
}