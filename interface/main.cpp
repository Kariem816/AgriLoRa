#include <raylib-cpp/raylib-cpp.hpp>

#include <iostream>

#include <camera.h>
#include <ui/window.h>
#include <widgets/hud.h>

int main()
{
    raylib::Window window(800, 600, "AgriLoRa");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    UiCamera camera;
    
    auto screenSize = window.GetSize();
    Hud hud;
    hud.SetPosition(raylib::Vector2::Zero());
    hud.SetSize(screenSize * raylib::Vector2{0.25f, 0.05f});
    hud.SubscribeOnMode2D([]()
                          { std::cout << "Switched to 2D mode" << std::endl; });
    hud.SubscribeOnMode3D([]()
                          { std::cout << "Switched to 3D mode" << std::endl; });
    bool debugMode = false;
    hud.SetDebug(debugMode);
    hud.SubscribeOnDebugToggle([&](bool isChecked)
                               { debugMode = isChecked; });

    Window::Instance().AddWidget(&hud);

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
        Window::Instance().Update(dt);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        camera.BeginMode();
        DrawModel(plot, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        if (debugMode)
        {
            DrawLine3D({10, 0, 0}, {-10, 0, 0}, RED);
            DrawLine3D({0, 10, 0}, {0, -10, 0}, GREEN);
            DrawLine3D({0, 0, 10}, {0, 0, -10}, BLUE);
            DrawGrid(30, 1.0f);
        }
        camera.EndMode();

        Window::Instance().Draw();

        EndDrawing();
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);
    auto size = window.GetSize();
    auto quitting = raylib::Text("Exiting...");
    quitting.SetFontSize(std::min(size.y, size.x) / 4);
    quitting.SetColor(WHITE);
    quitting.SetSpacing(quitting.GetFontSize() / 10);
    auto textSize = quitting.MeasureEx();
    quitting.Draw({(size.x - textSize.x) / 2, (size.y - textSize.y) / 2});
    EndDrawing();

    return 0;
}
