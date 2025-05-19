#include <SFML/Graphics.hpp>
#include "controller/InputController.hpp"
#include <iostream>

class MyCommand : public InputCommand
{
public:
    void execute(const InputEvent &event, const InputContext &context) override
    {
        if (event.type == InputEventType::MouseDragged)
            in_drug = true,
            std::cout << "dragged..." << std::endl;
        if (event.button == sf::Mouse::Left && event.type == InputEventType::MouseReleased && in_drug)
            in_drug = false,
            std::cout << "DONE" << std::endl; 
    }
private:
    bool in_drug = false;
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
        window.display();
    }

    return 0;
}