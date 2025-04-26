#pragma once
#include "Command.hpp"

#include "../EditorController.hpp"
#include "../InputBinding.hpp"


class MouseDownCommand : public Command
{
public:
    MouseDownCommand(EditorController *c, InputContext *ctx);
    void execute(const CommandContext &ctx) override;

private:
    EditorController *controller;
    InputContext *context;
};

class HoldCommand : public Command
{
public:
    HoldCommand(EditorController *c, InputContext *ctx);
    void execute(const CommandContext &ctx) override;
private:
    EditorController *controller;
    InputContext *context;
};

class MouseUpCommand : public Command
{
public:
    MouseUpCommand(EditorController *c, InputContext *ctx);
    void execute(const CommandContext &ctx) override;

private:
    EditorController *controller;
    InputContext *context;
};
