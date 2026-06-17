#include "drawable.h"

void Drawable::Draw()
{
    auto rect = GetRect();
    DrawRectangleRec(rect, bg_);
    if (frameThickness_ > 0)
    {
        DrawRectangleLinesEx(rect, frameThickness_, frameColor_);
    }
}

raylib::Rectangle Drawable::GetInnerRect() const
{
    auto rect = GetRect();
    if (rect.width <= 0 || rect.height <= 0)
    {
        return rect;
    }
    
    if (frameThickness_ > 0)
    {
        rect.x += frameThickness_;
        rect.y += frameThickness_;
        rect.width -= frameThickness_ * 2;
        rect.height -= frameThickness_ * 2;
    }
    return rect;
}