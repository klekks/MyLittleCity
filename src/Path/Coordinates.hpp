#pragma once

#include <cstdint>
#include <cmath>


struct Vec2 
{
    float x, y;

    float distanceTo(const Vec2& other) const;
    bool operator==(const Vec2& other) const;
};


struct Vec2Hash 
{
    std::size_t operator()(const Vec2& v) const;
};


struct Road 
{
    int id;
    Vec2 start;
    Vec2 end;
    float length() const;
};