#include "clickable.h"

void Clickable::Update(float dt)
{
    Hoverable::Update(dt);
    if (IsMouseOver() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        isClicked_ = true;
        TriggerOnClick(GetMousePosition());
    }
}

bool Clickable::IsClicked() const
{
    return isClicked_;
}