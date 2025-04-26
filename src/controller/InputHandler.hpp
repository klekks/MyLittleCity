#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

#include "InputBinding.hpp"
#include "commands/Command.hpp"

class InputHandler
{
public:
    void bind(const InputBinding &binding, std::shared_ptr<Command> command);
    void handleInput(const sf::Event &event);
    void handleHold(sf::RenderWindow &window);
private:
    std::unordered_map<InputBinding, std::shared_ptr<Command>> bindings;
    std::unordered_map<sf::Mouse::Button, bool> mouseState;
};
