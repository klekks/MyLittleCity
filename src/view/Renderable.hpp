#include <SFML/Graphics.hpp>
#include "../core/Entity.hpp"

class Renderable : virtual public Entity
{
public:
    virtual void render(sf::RenderTarget &target) = 0;
    bool is_renderable() const override { return true; }
    virtual ~Renderable() = default;
};
