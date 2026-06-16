#include "ui.h"

UiElement::UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size)
    : position_(position), size_(size) {}
UiElement::UiElement(const raylib::Rectangle &rect)
    : position_(rect.x, rect.y), size_(rect.width, rect.height) {}
UiElement::UiElement(const raylib::Vector2 &position, const raylib::Vector2 &size, const raylib::Color &color, bool hasFrame)
    : position_(position), size_(size), color_(color), hasFrame_(hasFrame) {}

UiElement::~UiElement()
{
    TriggerOnDestroy();
    for (auto &child : children_)
    {
        delete child.second;
    }
}

void UiElement::Update(float dt)
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

    if (isOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        TriggerOnClick(mousePos);
    }

    for (auto &[id, child] : children_)
    {
        child->Update(dt);
    }
}

void UiElement::Draw()
{
    if (hasFrame_)
    {
        DrawRectangleV(position_ + raylib::Vector2{1, 1}, size_ - raylib::Vector2{2, 2}, color_);
        DrawRectangleLinesEx(GetRect(), 1, frameColor_);
    }
    else
    {
        DrawRectangleV(position_, size_, color_);
    }

    for (const auto &[id, child] : children_)
    {
        child->Draw();
    }
}