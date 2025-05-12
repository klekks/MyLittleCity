#pragma once
#include <SFML/Window.hpp>
#include <array>
#include <set>
#include <unordered_map>

enum class InputEventType
{
    KeyPressed,
    KeyReleased,
    MousePressed,
    MouseReleased,
    MouseMoved,
    MouseDragged,
    MouseScrolled
};

struct InputEvent
{
    InputEventType type;
    sf::Keyboard::Key key = sf::Keyboard::Unknown;
    sf::Mouse::Button button = sf::Mouse::Button::ButtonCount;
    sf::Vector2i position;
    float delta = 0.f;
};

struct InputContext
{
    std::array<bool, sf::Keyboard::KeyCount> keyDown{};
    std::set<sf::Mouse::Button> mouseButtonsDown;

    sf::Vector2i mousePosition;
    sf::Vector2i lastMousePosition;

    bool isKeyDown(sf::Keyboard::Key key) const
    {
        return keyDown[key];
    }

    bool isMouseDown(sf::Mouse::Button button) const
    {
        return mouseButtonsDown.contains(button);
    }

    sf::Vector2i mouseDelta() const
    {
        return mousePosition - lastMousePosition;
    }
};

class InputCommand
{
public:
    virtual ~InputCommand() = default;
    virtual void execute(const InputEvent &event, const InputContext &context) = 0;
};

class InputController
{
public:
    void handleEvent(const sf::Event &event);
    void update();

    void bind(InputEventType type, InputCommand *command);
    void bindDrag(sf::Mouse::Button button, InputCommand *command);

    const InputContext &getContext() const;

private:
    std::unordered_map<InputEventType, std::vector<InputCommand *>> commands;
    std::unordered_map<sf::Mouse::Button, std::vector<InputCommand *>> dragCommands;

    InputContext context;
};
