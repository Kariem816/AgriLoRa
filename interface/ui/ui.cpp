#include "ui.h"

UiElement::UiElement() : position_(0, 0), size_(0, 0) {}
UiElement::UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size)
    : position_(position), size_(size) {}
UiElement::UiElement(const raylib::Rectangle &rect)
    : position_(rect.x, rect.y), size_(rect.width, rect.height) {}

UiElement::~UiElement()
{
    TriggerOnDestroy();
}

raylib::Rectangle UiElement::GetRect() const
{
    return {position_, size_};
}

raylib::Vector2 UiElement::GetCenter() const
{
    return {position_.x + size_.x / 2, position_.y + size_.y / 2};
}