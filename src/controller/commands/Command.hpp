#pragma once

#include <SFML/Graphics.hpp>

struct CommandContext
{
    sf::Vector2i mousePosition;
};

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(const CommandContext &ctx) = 0;
};
