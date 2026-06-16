#include "btn.h"

#include <raylib-cpp/raylib-cpp.hpp>

void Btn::Draw()
{
    UiElement::Draw();
    if (IsMouseOver())
    {
        raylib::Color overlay{GetColor()};
        overlay.a = 100;
        DrawRectangleRec(GetRect(), overlay);
    }

    if (!label_.empty())
    {
        raylib::Text text(label_, fontSize_, BLACK);
        raylib::Vector2 textSize = text.Measure();
        auto pos = GetPosition();
        auto size = GetSize();
        raylib::Vector2 textPos = {pos.x + (size.x - textSize.x) / 2, pos.y + (size.y - textSize.y) / 2};
        text.Draw(textPos);
    }
}
