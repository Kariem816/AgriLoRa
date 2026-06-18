#include "hoverable.h"

void Hoverable::Update(float)
{
    raylib::Vector2 mousePos = GetMousePosition();
    bool isOver = CheckCollisionPointRec(mousePos, GetRect());

    if (isOver && !isMouseOver_)
    {
        isMouseOver_ = true;
        TriggerOnEnter();
    }
    else if (!isOver && isMouseOver_)
    {
        isMouseOver_ = false;
        TriggerOnLeave();
    }
}

bool Hoverable::IsMouseOver() const
{
    return isMouseOver_;
}
