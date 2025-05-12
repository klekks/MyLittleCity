#include <SFML/Graphics.hpp>

class Renderable
{
public:
    virtual void render(sf::RenderTarget &target) = 0;
    virtual ~Renderable() = default;
};
