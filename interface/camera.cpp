#include "camera.h"
#include "utils/easing.h"

#define MOUSE_SENSITIVITY (0.1f)

UiCamera::UiCamera()
{
    upVector = {0.0f, 1.0f, 0.0f};
    target = {0.0f, 0.0f, 0.0f};
    camera.SetTarget(target);
    camera.SetUp(upVector);
    camera.SetFovy(45.0f);
    camera.SetProjection(CAMERA_PERSPECTIVE);
    updateCameraFromSpherical();
}
UiCamera::~UiCamera() = default;

void UiCamera::Update(float dt)
{
    updateAnimations(dt);
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

void UiCamera::Orbit(float dx, float dy)
{
    if (isAnimating())
        return;

    yaw += dx * MOUSE_SENSITIVITY;

    pitch -= dy * MOUSE_SENSITIVITY;
    pitch = Clamp(pitch, -89.0f, 89.0f);
}

void UiCamera::Zoom(float amount)
{
    if (isAnimating())
        return;

    radius -= amount * radius * 0.1f;
    radius = Clamp(radius, 0.5f, 100.0f);
}

void UiCamera::Pan(float dx, float dy)
{
    if (isAnimating())
        return;

    auto forward =
        (raylib::Vector3(camera.GetTarget()) - camera.GetPosition()).Normalize();

    auto right = forward.CrossProduct(camera.GetUp()).Normalize();
    auto up = right.CrossProduct(forward).Normalize();

    auto pan =
        right * (-dx * MOUSE_SENSITIVITY) +
        up * (dy * MOUSE_SENSITIVITY);

    target += pan;
}

void UiCamera::ResetView()
{
    if (isAnimating())
        return;

    yaw.to(defaultYaw, 0.5f, Easing::EaseOutSine);
    pitch.to(defaultPitch, 0.5f, Easing::EaseOutSine);
    radius.to(defaultRadius, 0.5f, Easing::EaseOutSine);

    target.to({0.0f, 0.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
    upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);

    mode2D = false;
}

void UiCamera::SetTopView()
{
    if (isAnimating())
        return;

    pitch.to(89.9f, 0.5f, Easing::EaseOutSine);
    upVector.to({0.0f, 0.0f, -1.0f}, 0.5f, Easing::EaseOutSine);

    mode2D = true;
}

void UiCamera::SetPerspectiveView()
{
    if (isAnimating())
        return;

    pitch.to(defaultPitch, 0.5f, Easing::EaseOutSine);
    yaw.to(defaultYaw, 0.5f, Easing::EaseOutSine);
    upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);

    mode2D = false;
}

void UiCamera::SetAxisView(AxisView axis)
{
    if (isAnimating())
        return;

    if (axis == AxisView::X)
    {
        yaw.to(0.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 0.0f : defaultPitch, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
    }
    else if (axis == AxisView::Y)
    {
        yaw.to(90.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 89.9f : defaultPitch, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 0.0f, -1.0f}, 0.5f, Easing::EaseOutSine);
    }
    else if (axis == AxisView::Z)
    {
        yaw.to(-90.0f, 0.5f, Easing::EaseOutSine);
        pitch.to(mode2D ? 0.0f : defaultPitch, 0.5f, Easing::EaseOutSine);
        upVector.to({0.0f, 1.0f, 0.0f}, 0.5f, Easing::EaseOutSine);
    }
}

void UiCamera::updateAnimations(float dt)
{
    yaw.update(dt);
    pitch.update(dt);
    radius.update(dt);
    upVector.update(dt);
    target.update(dt);
}

bool UiCamera::isAnimating() const
{
    return yaw.isAnimating() || pitch.isAnimating() || radius.isAnimating() || upVector.isAnimating() || target.isAnimating();
}

void UiCamera::updateCameraFromSpherical()
{
    float p = pitch * DEG2RAD;
    float y = yaw * DEG2RAD;
    raylib::Vector3 target = this->target;

    camera.SetPosition({target.x + radius * cosf(p) * cosf(y),
                        target.y + radius * sinf(p),
                        target.z + radius * cosf(p) * sinf(y)});
    camera.SetTarget(target);
    camera.SetUp(upVector);
}
