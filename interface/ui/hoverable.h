#pragma once

#include <ui/ui.h>

class Hoverable : public virtual UiElement
{
public:
    Hoverable() = default;
    Hoverable(const raylib::Vector2 &position, const raylib::Vector2 &size)
        : UiElement(position, size) {}

    virtual void Update(float dt) override;
    bool IsMouseOver() const;

    EVENTSOURCEV(enterEv, OnEnter)
    EVENTSOURCEV(leaveEv, OnLeave)

private:
    bool isMouseOver_ = false;
};
