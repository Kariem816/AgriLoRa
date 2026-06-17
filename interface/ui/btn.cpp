#include "btn.h"

#include <raylib-cpp/raylib-cpp.hpp>

void Btn::Update(float dt)
{
    Clickable::Update(dt);
    Label::Update(dt);
}

void Btn::Draw()
{
    Drawable::Draw();
    Label::Draw();
    if (IsMouseOver())
    {
        raylib::Color overlay{GetBackground()};
        overlay.a = 100;
        DrawRectangleRec(GetRect(), overlay);
    }
}
