#include "checkbox.h"

#include <iostream>

Checkbox::Checkbox()
    : UiElement(), Label("")
{
    updateLayout();
}

Checkbox::Checkbox(const raylib::Vector2 &position, const raylib::Vector2 &size, const std::string &label)
    : Clickable(position, size), Label(label)
{
    updateLayout();
}

void Checkbox::Update(float dt)
{
    Clickable::Update(dt);

    if (IsClicked())
    {
        checked_ = !checked_;
        checkEv.Trigger(checked_);
    }
}

void Checkbox::Draw()
{
    Drawable::Draw();

    DrawRectangleRec(checkboxRect_, checkBg_);
    DrawRectangleLinesEx(checkboxRect_, 1, checkColor_);
    if (IsMouseOver())
    {
        raylib::Color overlay = checkBg_;
        overlay.a = 100;
        DrawRectangleRec(GetRect(), overlay);
    }

    raylib::Text text = GetLabel();
    text.SetColor(GetLabelColor());
    text.SetFontSize(GetFontSize());
    text.SetSpacing(GetFontSpacing());
    auto textSize = text.MeasureEx();
    auto textPosY = labelRect_.y + (labelRect_.height - textSize.y) * 0.5f;
    text.Draw({labelRect_.x, textPosY});

    if (checked_)
    {
        // draw check mark
        auto origin = raylib::Vector2{checkboxRect_.x + checkboxRect_.width * 0.5f, checkboxRect_.y + checkboxRect_.height * (1 - checkboxPadding_)};
        auto thickness = std::min(checkboxRect_.width, checkboxRect_.height) * 0.1f;

        // short line from mid-left to origin
        auto midLeft = raylib::Vector2{checkboxRect_.x + checkboxRect_.width * checkboxPadding_, checkboxRect_.y + checkboxRect_.height * 0.5f};
        DrawLineEx(midLeft, origin, thickness, checkColor_);

        // long line from top-right to origin
        auto topRight = raylib::Vector2{checkboxRect_.x + checkboxRect_.width * (1 - checkboxPadding_), checkboxRect_.y + checkboxRect_.height * checkboxPadding_};
        DrawLineEx(topRight, origin, thickness, checkColor_);
    }
}

void Checkbox::SetPosition(raylib::Vector2 position)
{
    UiElement::SetPosition(position);
    updateLayout();
}

void Checkbox::SetSize(raylib::Vector2 size)
{
    UiElement::SetSize(size);
    updateLayout();
}

void Checkbox::SetLabelSize(float value)
{
    labelSize_ = value;
    updateLayout();
}

void Checkbox::SetGap(float value)
{
    gap_ = value;
    updateLayout();
}

void Checkbox::updateLayout()
{
    auto inner = GetInnerRect();

    float gap = inner.width * gap_;
    float labelSize = inner.width * labelSize_;
    float checkSize = inner.width - labelSize - gap;

    checkboxRect_ = raylib::Rectangle{inner.x, inner.y + (inner.height - checkSize) / 2, checkSize, checkSize};
    labelRect_ = raylib::Rectangle{inner.x + checkSize + gap, inner.y, labelSize, inner.height};
}