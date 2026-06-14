#include <raylib-cpp/raylib-cpp.hpp>
#include "utils/anim.h"

int main() {
    raylib::Window window(800, 600, "AgriLoRa UI");
    window.SetFullscreen(true);
    
    raylib::Camera3D camera;
    camera.SetPosition({10.0f, 10.0f, 10.0f});
    camera.SetTarget({0.0f, 0.0f, 0.0f});
    camera.SetUp({0.0f, 1.0f, 0.0f});
    camera.SetFovy(45.0f);
    camera.SetProjection(CAMERA_PERSPECTIVE);

    raylib::Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
    AnimatableFloat cubeX(-5.0f);
    AnimatableFloat cubeZ(-5.0f);
    cubeX.to(5.0f, 1.0f, Easing::EaseInSine);
    cubeZ.to(5.0f, 4.0f, Easing::EaseInSine);
    cubeX.setLoop(LoopMode::Reverse);
    cubeZ.setLoop(LoopMode::Reverse);

    DisableCursor();
    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        float dt = GetFrameTime();
        camera.Update(CAMERA_FREE);
        
        cubeX.update(dt);
        cubeZ.update(dt);
        cubePosition.SetX(cubeX.value());
        cubePosition.SetZ(cubeZ.value());

        if (IsKeyPressed(KEY_Z)) {
            camera.SetTarget({0.0f, 0.0f, 0.0f});
        }
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        camera.BeginMode();
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

        DrawGrid(10, 1.0f);
        camera.EndMode();

        DrawRectangle(10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(10, 10, 320, 93, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);

        EndDrawing();
    }
    return 0;
}