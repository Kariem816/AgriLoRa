#include "drawable.h"

void Drawable::Draw()
{
    auto rect = GetRect();
    if (frameThickness_ > 0)
    {
        DrawRectangleLinesEx(rect, frameThickness_, frameColor_);
        auto pos = GetPosition() + raylib::Vector2::One() * frameThickness_ * 0.5;
        auto size = GetSize() - raylib::Vector2::One() * frameThickness_;
        DrawRectangleV(pos, size, bg_);
    }
    else
    {
        DrawRectangleRec(rect, bg_);
    }
}