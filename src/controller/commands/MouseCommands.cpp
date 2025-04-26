#include "MouseCommands.hpp"

MouseDownCommand::MouseDownCommand(EditorController *c, InputContext *ctx)
    : controller(c), context(ctx)
{
}

void MouseDownCommand::execute(const CommandContext &ctx) 
{
    if (context->getMode() == EditorMode::DrawRoad)
        controller->startRoad(ctx.mousePosition);
    else if (context->getMode() == EditorMode::PlaceBuilding)
        controller->placeBuilding(ctx.mousePosition);
}

HoldCommand::HoldCommand(EditorController *c, InputContext *ctx) : controller(c), context(ctx) {}

void HoldCommand::execute(const CommandContext &ctx) 
{
    if (context->getMode() == EditorMode::DrawRoad)
        controller->previewRoad(ctx.mousePosition);
}

MouseUpCommand::MouseUpCommand(EditorController *c, InputContext *ctx)
    : controller(c), context(ctx) {}

void MouseUpCommand::execute(const CommandContext &ctx) 
{
    if (context->getMode() == EditorMode::DrawRoad)
        controller->finishRoad(ctx.mousePosition);
    else if (context->getMode() == EditorMode::RemoveRoad)
        controller->removeRoad();
}
