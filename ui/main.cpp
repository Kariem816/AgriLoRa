#include <raylib-cpp/raylib-cpp.hpp>

int main() {
    raylib::Window window(800, 600, "AgriLoRa UI");
    while (!window.ShouldClose()) {
        BeginDrawing();
        window.ClearBackground(raylib::Color::LightGray());
        EndDrawing();
    }
    return 0;
}