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

    virtual void Update(float dt);
    virtual void Draw();

    VGETTERSETTER(raylib::Vector2, Position, position_)
    VGETTERSETTER(raylib::Vector2, Size, size_)
    VGETTERSETTER(raylib::Color, Color, color_)
    VGETTERSETTER(bool, Frame, hasFrame_)
    VGETTERSETTER(raylib::Color, FrameColor, frameColor_)

    raylib::Rectangle GetRect() const
    {
        return {position_, size_};
    }

    raylib::Vector2 GetCenter() const
    {
        return {position_.x + size_.x / 2, position_.y + size_.y / 2};
    }

    UiElement *AddChild(const std::string &name, UiElement *child)
    {
        children_[name] = child;
        return child;
    }

    UiElement *GetChild(const std::string &name) const
    {
        auto it = children_.find(name);
        if (it != children_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    bool IsMouseOver() const
    {
        return isMouseOver_;
    }

private:
    raylib::Vector2 position_;
    raylib::Vector2 size_;
    raylib::Color color_{0x80181818};
    bool hasFrame_ = false;
    raylib::Color frameColor_{BLACK};

    std::map<std::string, UiElement *> children_;

    // Event sources for mouse interactions
    bool isMouseOver_ = false;

public:
    EVENTSOURCEV(enterEv, OnEnter)
    EVENTSOURCEV(leaveEv, OnLeave)
    EVENTSOURCE(clickEv, OnClick, raylib::Vector2)
    EVENTSOURCEV(destroyEv, OnDestroy)
    // EventSource<> onEnter_;
    // EventSource<> onLeave_;
    // EventSource<raylib::Vector2> onClick_;
    // EventSource<> onDestroy_;
};
