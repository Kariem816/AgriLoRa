#pragma once

#include <ui/macros.h>
#include <ui/hoverable.h>

class Clickable : public Hoverable
{
public:
    Clickable() = default;
    Clickable(const raylib::Vector2 &position, const raylib::Vector2 &size)
        : UiElement(position, size) {}

    void Update(float dt) override;
    bool IsClicked() const;

    EVENTSOURCE(clickEv, OnClick, raylib::Vector2)

private:
    bool isClicked_ = false;
};
