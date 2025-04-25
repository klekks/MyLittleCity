#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Command.hpp"

class Controller
{
public:
    virtual std::unique_ptr<Command<sf::Event>> handle_input(sf::Window& window, const sf::Event& event) = 0;
};