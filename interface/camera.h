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
    void blockInteraction(bool block);

private:
    raylib::Camera3D camera;
    raylib::Vector3 cameraTarget{0.0f, 0.0f, 0.0f};
    Animatable<float> yaw = 45.0f;     // horizontal angle (degrees)
    Animatable<float> pitch = 35.0f;   // vertical angle (degrees)
    Animatable<float> radius = 17.32f; // distance from target (≈ length of {10,10,10})
    bool mode2D = false;
    bool interactionBlocked = false; // Whether user interaction is currently blocked (e.g., during animation)

    Animatable<raylib::Vector3> upVector; // Up vector for the camera
};