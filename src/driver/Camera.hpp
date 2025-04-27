#pragma once


constexpr float ZOOM_STEP = 0.1f;
constexpr float MIN_ZOOM = 0.1f;
constexpr float MAX_ZOOM = 10.0f;


class Camera
{
public:
    Camera();
    void zoomIn();
    void zoomOut();
    float getZoom() const;

private:
    float zoomLevel;
};
