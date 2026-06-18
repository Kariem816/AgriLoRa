#pragma once

#include <raylib-cpp/raylib-cpp.hpp>
#include "utils/anim.h"

enum class AxisView
{
    X,
    Y,
    Z
};

class UiCamera {
public:
    UiCamera();
    ~UiCamera();

    void Update(float dt);
    void BeginMode();
    void EndMode();

    void Orbit(float dx, float dy);
    void Pan(float dx, float dy);
    void Zoom(float amount);
    void ResetView();
    void SetTopView();
    void SetPerspectiveView();
    void SetAxisView(AxisView axis);

private:
    void updateAnimations(float dt);
    void updateCameraFromSpherical();
    bool isAnimating() const;

private:
    raylib::Camera3D camera;

    // Spherical coordinates for the camera position
    static constexpr float defaultYaw = 45.0f;     // horizontal angle (degrees)
    Animatable<float> yaw = defaultYaw;
    static constexpr float defaultPitch = 35.0f;   // vertical angle (degrees)
    Animatable<float> pitch = defaultPitch;
    static constexpr float defaultRadius = 17.32f; // distance from target (≈ length of {10,10,10})
    Animatable<float> radius = defaultRadius;
    
    // state
    bool mode2D = false;
    
    Animatable<raylib::Vector3> upVector; // Up vector for the camera
    Animatable<raylib::Vector3> target; // Camera target point (the point the camera looks at)
};