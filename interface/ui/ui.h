#pragma once

#include <raylib-cpp/raylib-cpp.hpp>

#include <utils/event-src.h>
#include <ui/macros.h>
#include <ui/base.h>

class UiElement : public UiBase
{
public:
    UiElement() = default;
    UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size);
    UiElement(const raylib::Rectangle &rect);
    ~UiElement();

    VGETTERSETTER(raylib::Vector2, Position, position_)
    VGETTERSETTER(raylib::Vector2, Size, size_)
    

    raylib::Rectangle GetRect() const;
    raylib::Vector2 GetCenter() const;

private:
    raylib::Vector2 position_;
    raylib::Vector2 size_;

public:
    EVENTSOURCEV(destroyEv, OnDestroy)
};
