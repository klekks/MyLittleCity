#pragma once

class Point
{
    float x = 0, y = 0;

public:
    Point(float x, float y);

    bool operator<(const Point&) const;
    bool operator==(const Point&) const;

    float distance(const Point&) const;
};
