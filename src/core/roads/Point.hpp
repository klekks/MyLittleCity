#pragma once

#include <SFML/Graphics.hpp>

struct Point
{
    float x = 0, y = 0;
public:
    Point(float x, float y);
    Point(const std::initializer_list<float> &list);

    bool operator<(const Point&) const;
    bool operator==(const Point&) const;

    Point operator+(const Point&) const;
    Point operator-(const Point&) const;
    Point operator*(float) const;


    float distance(const Point&) const;
    float distanceX(const Point&) const;
    float distanceY(const Point&) const;

    float getX() const;
    float getY() const;

    float dot(const Point &other) const;
    sf::Vector2f to_sfml_vector2f() const;
};
