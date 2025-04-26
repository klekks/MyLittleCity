#include "RoadView.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>

RoadView::RoadView(std::shared_ptr<Intersection> a, std::shared_ptr<Intersection> b)
    : Road(a, b)
{}


void RoadView::draw(sf::RenderWindow &window) const
{
    const float width = 20.0f;
    const float halfWidth = width / 2.0f;

    auto begin_intersection = this->begin_intersection.lock();
    auto end_intersection = this->end_intersection.lock();

    if (!begin_intersection || !end_intersection)
        return;


    float dx = begin_intersection->distanceX(*end_intersection);
    float dy = begin_intersection->distanceY(*end_intersection);
    float length = this->length();

    float nx = dx / length;
    float ny = dy / length;
    float px = -ny;
    float py = nx;

    // точки по краям дороги
    sf::VertexArray roadShape(sf::Quads, 4);
    roadShape[0].position = (begin_intersection->coordinates() + Point{px * halfWidth, py * halfWidth}).to_sfml_vector2f();
    roadShape[1].position = (end_intersection->coordinates()  + Point{px * halfWidth, py * halfWidth}).to_sfml_vector2f(); 
    
    roadShape[2].position = (end_intersection->coordinates()  + Point{-px * halfWidth, -py * halfWidth}).to_sfml_vector2f(); 
    roadShape[3].position = (begin_intersection->coordinates()  + Point{-px * halfWidth, -py * halfWidth}).to_sfml_vector2f(); 

    for (int i = 0; i < 4; ++i)
    {
        roadShape[i].color = sf::Color(68, 68, 68); // #444
    }

    window.draw(roadShape);

    // линии по краям
    const float midOffset = 3.0f;
    float offsetX = -ny * midOffset;
    float offsetY = nx * midOffset;

    sf::VertexArray edgeLine1(sf::Lines, 2);
    edgeLine1[0].position = (begin_intersection->coordinates() + Point{offsetX, offsetY}).to_sfml_vector2f();
    edgeLine1[1].position = (end_intersection->coordinates() + Point{offsetX, offsetY}).to_sfml_vector2f();
    edgeLine1[0].color = edgeLine1[1].color = sf::Color(136, 136, 136); // #888

    sf::VertexArray edgeLine2(sf::Lines, 2);
    edgeLine2[0].position = (begin_intersection->coordinates() - Point{offsetX, offsetY}).to_sfml_vector2f();
    edgeLine2[1].position = (end_intersection->coordinates() - Point{offsetX, offsetY}).to_sfml_vector2f();
    edgeLine2[0].color = edgeLine2[1].color = sf::Color(136, 136, 136); // #888

    window.draw(edgeLine1);
    window.draw(edgeLine2);

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

        sf::Vector2f startPoint{
            (begin_intersection->coordinates() * (1 - startFrac) + end_intersection->coordinates() * startFrac)
                .to_sfml_vector2f()};
        sf::Vector2f endPoint{
            (begin_intersection->coordinates() * (1 - endFrac) + end_intersection->coordinates() * endFrac)
                .to_sfml_vector2f()};

        dashedLine.append(sf::Vertex(startPoint, sf::Color::White));
        dashedLine.append(sf::Vertex(endPoint, sf::Color::White));
    }

    window.draw(dashedLine);

    // // Подпись дороги
    // static sf::Font font;
    // static bool loaded = false;
    // if (!loaded && !(loaded = font.loadFromFile("./ArialRegular.ttf")))
    // {
    //     std::cout << "FAILED TO LOAD FONT" << std::endl;
    // }

    sf::Text text;
    // text.setFont(font);
    text.setString("R" + std::to_string(id()));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Regular);
    text.setPosition(
        (begin_intersection->getX() + end_intersection->getX()) / 2.0f,
        (begin_intersection->getY() + end_intersection->getY()) / 2.0f - 8.0f);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);

    window.draw(text);
}
