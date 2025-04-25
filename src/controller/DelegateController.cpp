#include "DelegateController.hpp"

#include "WindowController.hpp"
#include "MouseController.hpp"

class AgregatedCommand : public Command<sf::Event>
{
public:
    void execute(const sf::Event& event)
    {
        for (auto &command: commands)
        {
            command->execute(event);
        }
    }

    void push_command(std::unique_ptr<Command<sf::Event>> &&command)
    {
        if (command)
        {
            commands.push_back(std::move(command));
        }
    }

private:
    std::vector<std::unique_ptr<Command<sf::Event>>> commands;
};


void DelegateController::delegate_event_to_controller(sf::Event::EventType type, std::shared_ptr<Controller> controller)
{
    if (controller)
    {
        delegates[type].push_back(controller);
    }
}

std::unique_ptr<Command<sf::Event>> DelegateController::handle_input(sf::Window& window, const sf::Event& event)
{
    if (delegates.find(event.type) != delegates.end())
    {
        auto command = std::make_unique<AgregatedCommand>();
        for (auto& controller : delegates[event.type])
        {
            command->push_command(controller->handle_input(window, event));
        }
        return command;
    }
    return {};
}

std::unique_ptr<DelegateController> make_delegate_controller()
{
    auto delegate_controller = std::make_unique<DelegateController>();

    delegate_controller->delegate_event_to_controller(sf::Event::MouseButtonPressed, std::make_unique<MouseController>());
    delegate_controller->delegate_event_to_controller(sf::Event::Closed, std::make_unique<WindowController>());

    return delegate_controller;
}
