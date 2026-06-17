#pragma once

#include <ui/ui.h>

class Label : public virtual UiElement
{
public:
    Label(const std::string &label = "");
    Label(const raylib::Vector2 &position, const raylib::Vector2 &size, const std::string &label = "");
    Label(const raylib::Rectangle &rect, const std::string &label = "");

    VGETTERSETTER(std::string, Label, label_)
    VGETTERSETTER(raylib::Color, LabelColor, labelColor_)
    VGETTERSETTER(float, FontSize, fontSize_)
    VGETTERSETTER(float, FontSpacing, fontSpacing_)

    virtual void Update(float dt) override;
    virtual void Draw() override;

private:
    std::string label_;
    raylib::Color labelColor_ = BLACK;
    float fontSize_ = 20;
    float fontSpacing_ = 2.0f;

    raylib::Text text_;
};
