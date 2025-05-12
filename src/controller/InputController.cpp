#include "InputController.hpp"

void InputController::handleEvent(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        context.keyDown[event.key.code] = true;
        for (auto *cmd : commands[InputEventType::KeyPressed])
            cmd->execute({InputEventType::KeyPressed, event.key.code}, context);
        break;

    case sf::Event::KeyReleased:
        context.keyDown[event.key.code] = false;
        for (auto *cmd : commands[InputEventType::KeyReleased])
            cmd->execute({InputEventType::KeyReleased, event.key.code}, context);
        break;

    case sf::Event::MouseButtonPressed:
        context.mouseButtonsDown.insert(event.mouseButton.button);
        for (auto *cmd : commands[InputEventType::MousePressed])
            cmd->execute({InputEventType::MousePressed, sf::Keyboard::Unknown, event.mouseButton.button, {event.mouseButton.x, event.mouseButton.y}}, context);
        break;

    case sf::Event::MouseButtonReleased:
        context.mouseButtonsDown.erase(event.mouseButton.button);
        for (auto *cmd : commands[InputEventType::MouseReleased])
            cmd->execute({InputEventType::MouseReleased, sf::Keyboard::Unknown, event.mouseButton.button, {event.mouseButton.x, event.mouseButton.y}}, context);
        break;

    case sf::Event::MouseMoved:
    {
        context.lastMousePosition = context.mousePosition;
        context.mousePosition = {event.mouseMove.x, event.mouseMove.y};

        for (auto *cmd : commands[InputEventType::MouseMoved])
            cmd->execute({InputEventType::MouseMoved, sf::Keyboard::Unknown, sf::Mouse::Button::ButtonCount, context.mousePosition}, context);

        for (auto &btn : context.mouseButtonsDown)
        {
            for (auto *cmd : dragCommands[btn])
            {
                cmd->execute({InputEventType::MouseDragged, sf::Keyboard::Unknown, btn, context.mousePosition}, context);
            }
        }
        break;
    }

    case sf::Event::MouseWheelScrolled:
        for (auto *cmd : commands[InputEventType::MouseScrolled])
            cmd->execute({InputEventType::MouseScrolled, sf::Keyboard::Unknown, sf::Mouse::Button::ButtonCount, {event.mouseWheelScroll.x, event.mouseWheelScroll.y}, event.mouseWheelScroll.delta}, context);
        break;

    default:
        break;
    }
}

const InputContext &InputController::getContext() const
{
    return context;
}

void InputController::bind(InputEventType type, InputCommand *command)
{
    commands[type].push_back(command);
}

void InputController::bindDrag(sf::Mouse::Button button, InputCommand *command)
{
    dragCommands[button].push_back(command);
}