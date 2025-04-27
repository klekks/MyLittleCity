#include "InputHandler.hpp"

#include <memory>

void InputHandler::bind(const InputBinding &binding, std::shared_ptr<Command> command)
{
    bindings[binding] = command;
}

void InputHandler::handleInput(const sf::Event &event, sf::RenderWindow &window)
{
    CommandContext ctx;
    InputBinding binding;

    ctx.mousePosition = sf::Vector2i(window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}));

    if (event.type == sf::Event::MouseButtonPressed)
    {
        binding = {InputActionType::MouseDown, event.mouseButton.button};
        mouseState[event.mouseButton.button] = true;
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        binding = {InputActionType::MouseUp, event.mouseButton.button};
        mouseState[event.mouseButton.button] = false;
    }
    else
        return;

    auto it = bindings.find(binding);
    if (it != bindings.end())
        it->second->execute(ctx);
}

void InputHandler::handleHold(sf::RenderWindow &window)
{
    for (auto &[button, pressed] : mouseState)
    {
        if (pressed)
        {
            InputBinding binding = {InputActionType::Hold, button};
            auto it = bindings.find(binding);
            if (it != bindings.end())
            {
                CommandContext ctx;
                ctx.mousePosition = sf::Mouse::getPosition(window);
                ctx.mousePosition = sf::Vector2i(window.mapPixelToCoords(ctx.mousePosition));
                it->second->execute(ctx);
            }
        }
    }
}
