#pragma once

#include "../Path/Coordinates.hpp"

struct Building {
    int id;
    Vec2 position;
    int connectedRoadId = -1; // будет найден ближайший
};
