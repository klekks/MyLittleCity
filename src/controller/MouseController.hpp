#pragma once 
#include <memory>

#include <SFML/Graphics.hpp>

#include "Controller.hpp"
#include "Command.hpp"


class MouseController : public Controller
{
public:
    std::unique_ptr<Command<sf::Event>> handle_input(sf::Window& window, const sf::Event& event) override;
};
