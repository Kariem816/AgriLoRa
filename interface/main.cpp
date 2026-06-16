#include <raylib-cpp/raylib-cpp.hpp>

#include <camera.h>
#include <ui/hud.h>

int main()
{
    raylib::Window window(800, 600, "AgriLoRa UI");
    window.SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    UiCamera camera;
    auto screenSize = window.GetSize();
    Hud hud;
    hud.SetPosition(raylib::Vector2::Zero());
    hud.SetSize(screenSize * raylib::Vector2{1, 0.1f});

    raylib::Model plot("assets/obj/plot.glb");
    if (!plot.IsValid())
    {
        TraceLog(LOG_ERROR, "Failed to load model");
        return -1;
    }

    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        float dt = GetFrameTime();
        camera.Update();
        hud.Update(dt);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        camera.BeginMode();
        DrawModel(plot, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        DrawLine3D({10, 0, 0}, {-10, 0, 0}, RED);
        DrawLine3D({0, 10, 0}, {0, -10, 0}, GREEN);
        DrawLine3D({0, 0, 10}, {0, 0, -10}, BLUE);
        camera.EndMode();

        hud.Draw();

        EndDrawing();
    }
    return 0;
}
