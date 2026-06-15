#include "camera.h"
#include "utils/easing.h"

#define MOUSE_SENSITIVITY (0.1f)

UiCamera::UiCamera()
{
    upVector = {0.0f, 1.0f, 0.0f};
    camera.SetTarget({0.0f, 0.0f, 0.0f});
    camera.SetUp({0.0f, 1.0f, 0.0f});
    camera.SetFovy(45.0f);
    camera.SetProjection(CAMERA_PERSPECTIVE);
    updateCameraFromSpherical();
}
UiCamera::~UiCamera() = default;

void UiCamera::Update()
{
    float dt = GetFrameTime();
    yaw.update(dt);
    pitch.update(dt);
    radius.update(dt);
    upVector.update(dt);
    camera.SetUp(upVector);

    raylib::Vector2 mouseDelta = GetMouseDelta();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        yaw += mouseDelta.x * MOUSE_SENSITIVITY;
        pitch -= mouseDelta.y * MOUSE_SENSITIVITY;
        pitch = Clamp(pitch, -89.0f, 89.0f);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        raylib::Vector3 forward = (raylib::Vector3(camera.GetTarget()) - camera.GetPosition()).Normalize();
        auto right = forward.CrossProduct(camera.GetUp()).Normalize();
        auto up = right.CrossProduct(forward).Normalize();

        auto pan =
            (right * (-mouseDelta.x * MOUSE_SENSITIVITY)) + (up * (mouseDelta.y * MOUSE_SENSITIVITY));
        cameraTarget += pan;
        camera.SetTarget(cameraTarget);
    }

    raylib::Vector2 scroll = GetMouseWheelMoveV();
    radius -= scroll.y * radius * 0.1f;
    radius = Clamp(radius, 0.5f, 100.0f);

    if (IsKeyPressed(KEY_R))
    {
        yaw.to(45.0f, 0.2f, Easing::EaseOutSine);
        pitch.to(35.0f, 0.2f, Easing::EaseOutSine);
        radius.to(17.32f, 0.2f, Easing::EaseOutSine);
        cameraTarget = raylib::Vector3{0.0f, 0.0f, 0.0f};
        camera.SetTarget(cameraTarget);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
        mode2D = false;
    }

    if (IsKeyPressed(KEY_TWO))
    {
        pitch.to(89.9f, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 0.0f, -1.0f}, 0.5f, Easing::EaseOutSine);
        mode2D = true;
    }

    if (IsKeyPressed(KEY_THREE))
    {
        pitch.to(35.0f, 0.5f, Easing::EaseOutSine);
        yaw.to(45.0f, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
        mode2D = false;
    }

    if (IsKeyPressed(KEY_X))
    {
        yaw.to(0.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 0.0f : 35.0f, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
    }
    if (IsKeyPressed(KEY_Y))
    {
        yaw.to(90.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 89.9f : 35.0f, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 0.0f, -1.0f}, 0.5f, Easing::EaseOutSine);
    }
    if (IsKeyPressed(KEY_Z))
    {
        yaw.to(-90.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 0.0f : 35.0f, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
    }

    updateCameraFromSpherical();
}

void UiCamera::BeginMode()
{
    camera.BeginMode();
}
void UiCamera::EndMode()
{
    camera.EndMode();
}

void UiCamera::updateCameraFromSpherical()
{
    float p = pitch * DEG2RAD;
    float y = yaw * DEG2RAD;
    camera.SetPosition({cameraTarget.x + radius * cosf(p) * cosf(y),
                        cameraTarget.y + radius * sinf(p),
                        cameraTarget.z + radius * cosf(p) * sinf(y)});
}
