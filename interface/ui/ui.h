#pragma once

#include <raylib-cpp/raylib-cpp.hpp>

#include <utils/event-src.h>
#include <ui/macros.h>

class UiElement
{
public:
    UiElement() = default;
    UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size);
    UiElement(const raylib::Rectangle &rect);
    UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size, const raylib::Color &color, bool hasFrame = false);
    ~UiElement();

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

    VGETTERSETTER(raylib::Vector2, Position, position_)
    VGETTERSETTER(raylib::Vector2, Size, size_)
    VGETTERSETTER(raylib::Color, Color, color_)
    VGETTERSETTER(bool, Frame, hasFrame_)
    VGETTERSETTER(raylib::Color, FrameColor, frameColor_)

    raylib::Rectangle GetRect() const;
    raylib::Vector2 GetCenter() const;

private:
    raylib::Vector2 position_;
    raylib::Vector2 size_;
    raylib::Color color_{0x80181818};
    bool hasFrame_ = false;
    raylib::Color frameColor_{BLACK};

public:
    EVENTSOURCEV(destroyEv, OnDestroy)
};
