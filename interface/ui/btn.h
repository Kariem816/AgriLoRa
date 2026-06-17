#pragma once

#include <string>
#include <map>

#include <ui/ui.h>

class Btn : public UiElement
{
public:
    Btn(const std::string &label = "") : label_(label) {};
    Btn(const raylib::Vector2 &position, const raylib::Vector2 &size, const std::string &label = "")
        : UiElement(position, size), label_(label) {}
    Btn(const raylib::Rectangle &rect, const std::string &label = "")
        : UiElement({rect.x, rect.y}, {rect.width, rect.height}), label_(label) {}

    void Draw() override;

    GETTERSETTER(std::string, Label, label_)
    GETTERSETTER(raylib::Color, LabelColor, labelColor_)
    GETTERSETTER(float, FontSize, fontSize_)
    GETTERSETTER(float, FontSpacing, fontSpacing_)

private:
    std::string label_;
    raylib::Color labelColor_ = BLACK;
    float fontSize_ = 20;
    float fontSpacing_ = 2.0f;
};
