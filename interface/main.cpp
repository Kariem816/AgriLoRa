#include <raylib-cpp/raylib-cpp.hpp>

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

    hud.SubscribeOnMode2D([&]()
                          { camera.SetTopView(); });
    hud.SubscribeOnMode3D([&]()
                          { camera.SetPerspectiveView(); });

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
    Animatable<float> waterLevel = 0.2f;
    waterLevel.setLoop(LoopMode::Reverse);
    waterLevel.to(0.3f, 2.0f, Easing::EaseInOutSine);

    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        float dt = GetFrameTime();

        if (IsWindowResized())
        {
            auto newSize = window.GetSize();
            hud.SetSize(newSize * raylib::Vector2{0.25f, 0.05f});
        }

        // camera controls
        if (IsKeyPressed(KEY_R))
        {
            camera.ResetView();
        }
        else if (IsKeyPressed(KEY_X))
        {
            camera.SetAxisView(AxisView::X);
        }
        else if (IsKeyPressed(KEY_Y))
        {
            camera.SetAxisView(AxisView::Y);
        }
        else if (IsKeyPressed(KEY_Z))
        {
            camera.SetAxisView(AxisView::Z);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            auto mouseDelta = GetMouseDelta();
            camera.Orbit(mouseDelta.x, mouseDelta.y);
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            auto mouseDelta = GetMouseDelta();
            camera.Pan(-mouseDelta.x, mouseDelta.y);
        }

        auto mouseWheel = GetMouseWheelMoveV();
        camera.Zoom(mouseWheel.y);

        Window::Instance().Update(dt);
        camera.Update(dt);
        waterLevel.update(dt);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        camera.BeginMode();
        DrawModel(plot, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        DrawPlane({0.0f, waterLevel, 0.0f}, {23.0f, 5.0f}, BLUE);
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
