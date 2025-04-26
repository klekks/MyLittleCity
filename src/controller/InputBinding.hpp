#pragma once

#include <SFML/Window.hpp>

enum class InputActionType
{
    MouseDown,
    MouseUp,
    Hold
};

enum class EditorMode
{
    None,
    DrawRoad,
    PlaceBuilding,
    RemoveRoad
};

struct InputBinding
{
    InputActionType actionType;
    sf::Mouse::Button button;

    bool operator==(const InputBinding &other) const;
};

class InputContext
{
public:
    void setMode(EditorMode mode) { currentMode = mode; }
    EditorMode getMode() const { return currentMode; }
private:
    EditorMode currentMode = EditorMode::None;
};

namespace std
{
    template <>
    struct hash<InputBinding>
    {
        size_t operator()(const InputBinding &b) const;
    };
}