#include "Camera.hpp"

#include <cmath>

Camera::Camera() : zoomLevel(1.0f) {}

void Camera::zoomIn()
{
    zoomLevel = fminf(zoomLevel * (1 + ZOOM_STEP), MAX_ZOOM); 
}

void Camera::zoomOut()
{
    zoomLevel = fmaxf(zoomLevel * (1 - ZOOM_STEP), MIN_ZOOM); 
}

float Camera::getZoom() const
{
    return zoomLevel;
}
