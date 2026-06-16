#include <raylib-cpp/raylib-cpp.hpp>
#include "camera.h"

int main()
{
    raylib::Window window(800, 600, "AgriLoRa UI");
    window.SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    UiCamera camera;

    raylib::Model plot("assets/obj/plot.glb");
    if (!plot.IsValid())
    {
        TraceLog(LOG_ERROR, "Failed to load model");
        return -1;
    }

    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        camera.Update();

        BeginDrawing();
        ClearBackground(SKYBLUE);

        camera.BeginMode();
        DrawModel(plot, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        DrawLine3D({10, 0, 0}, {-10, 0, 0}, RED);
        DrawLine3D({0, 10, 0}, {0, -10, 0}, GREEN);
        DrawLine3D({0, 0, 10}, {0, 0, -10}, BLUE);
        camera.EndMode();

        EndDrawing();
    }
    return 0;
}
