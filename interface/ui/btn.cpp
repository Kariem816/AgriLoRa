#include "btn.h"

#include <raylib-cpp/raylib-cpp.hpp>

void Btn::Update(float dt)
{
    Clickable::Update(dt);
    Label::Update(dt);
}

void Btn::Draw()
{
    if (GetFrame())
    {
        DrawRectangleV(GetPosition() + raylib::Vector2{1, 1}, GetSize() - raylib::Vector2{2, 2}, GetColor());
        DrawRectangleLinesEx(GetRect(), 1, GetFrameColor());
    }
    else
    {
        DrawRectangleV(GetPosition(), GetSize(), GetColor());
    }

    Label::Draw();
    if (IsMouseOver())
    {
        raylib::Color overlay{GetColor()};
        overlay.a = 100;
        DrawRectangleRec(GetRect(), overlay);
    }
}
