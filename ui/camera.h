#pragma once

#include <raylib-cpp/raylib-cpp.hpp>
#include "utils/anim.h"

class UiCamera {
public:
    UiCamera();
    ~UiCamera();

    void Update();
    void BeginMode();
    void EndMode();

private:
    void updateCameraFromSpherical();

private:
    raylib::Camera3D camera;
    raylib::Vector3 cameraTarget{0.0f, 0.0f, 0.0f};
    Animatable<float> yaw = 45.0f;     // horizontal angle (degrees)
    Animatable<float> pitch = 35.0f;   // vertical angle (degrees)
    Animatable<float> radius = 17.32f; // distance from target (≈ length of {10,10,10})
    bool mode2D = false;

    Animatable<raylib::Vector3> upVector; // Up vector for the camera
};