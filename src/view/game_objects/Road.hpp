#pragma once
#include "../Renderable.hpp"
#include "../../game_objects/Road.hpp"

#include <array>

extern std::array<Point2Df, 4> get_corners(const Point2Df &center, const Point2Df &sizes, float rotation);

class RoadView : public Road, public Renderable
{
public:
    RoadView(std::shared_ptr<Crossroad> from, std::shared_ptr<Crossroad> to, float width = 15.f)
    :   Road(from, to, width)
    {}

    void render(sf::RenderTarget& target) const override
    {
        auto delta = from->coordinates() - to->coordinates();
        float dx = delta.x, dy = delta.y;

        float length = from->coordinates().length(to->coordinates());

        float nx = dx / length;
        float ny = dy / length;

        sf::VertexArray roadShape(sf::Quads, 4);
        auto collider = dynamic_cast<const BoundingBoxCollider*>(get_collider());
        auto corners = get_corners(collider->get_center(), collider->get_sizes(), collider->get_rotation());
        for (int i = 0; i < 4; ++i)
        {
            roadShape[i].position = {corners[i].x, corners[i].y};
            roadShape[i].color = sf::Color(68, 68, 68); // #444
        }

        target.draw(roadShape);

        // линии по краям
        const float midOffset = 3.0f;
        float offsetX = -ny * midOffset;
        float offsetY = nx * midOffset;
        sf::VertexArray edgeLine1(sf::Lines, 2);
        auto a = (from->coordinates() + Point2Df{offsetX, offsetY}), b = (to->coordinates() + Point2Df{offsetX, offsetY});
        edgeLine1[0].position = {a.x, a.y};
        edgeLine1[1].position = {b.x, b.y};
        edgeLine1[0].color = edgeLine1[1].color = sf::Color(136, 136, 136); // #888

        auto c = (from->coordinates() - Point2Df{offsetX, offsetY}), d = (to->coordinates() - Point2Df{offsetX, offsetY});
        sf::VertexArray edgeLine2(sf::Lines, 2);
        edgeLine2[0].position = {c.x, c.y};
        edgeLine2[1].position = {d.x, d.y};
        edgeLine2[0].color = edgeLine2[1].color = sf::Color(136, 136, 136); // #888

        target.draw(edgeLine1);
        target.draw(edgeLine2);

        // Центральная пунктирная линия
        sf::VertexArray dashedLine(sf::Lines);
        const float dashLength = 10.0f;
        const float gapLength = 10.0f;
        float totalLength = length;
        int numDashes = static_cast<int>(totalLength / (dashLength + gapLength));

        for (int i = 0; i < numDashes; ++i)
        {
            float startFrac = (i * (dashLength + gapLength)) / totalLength;
            float endFrac = (i * (dashLength + gapLength) + dashLength) / totalLength;

            if (endFrac > 1.0f)
                endFrac = 1.0f;

            auto a = (from->coordinates() * (1 - startFrac) + to->coordinates() * startFrac), 
                b = (from->coordinates() * (1 - endFrac) + to->coordinates() * endFrac);
            sf::Vector2f startPoint{
                a.x, a.y
            };
            sf::Vector2f endPoint{
                b.x, b.y
                };

            dashedLine.append(sf::Vertex(startPoint, sf::Color::White));
            dashedLine.append(sf::Vertex(endPoint, sf::Color::White));
        }

        target.draw(dashedLine);
    }
};