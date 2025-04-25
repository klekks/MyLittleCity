#include "Controller.hpp"

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <memory>
#include <vector>

class DelegateController : public Controller
{
    std::unordered_map<sf::Event::EventType, std::vector<std::shared_ptr<Controller>>> delegates;

public:

    void delegate_event_to_controller(sf::Event::EventType type, std::shared_ptr<Controller> controller);

    std::unique_ptr<Command<sf::Event>> handle_input(sf::Window& window, const sf::Event& event);
};


std::unique_ptr<DelegateController> make_delegate_controller();
