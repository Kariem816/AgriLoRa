#include "label.h"

Label::Label(const std::string &label) : label_(label) {};
Label::Label(const raylib::Vector2 &position, const raylib::Vector2 &size, const std::string &label)
    : UiElement(position, size), label_(label) {}
Label::Label(const raylib::Rectangle &rect, const std::string &label)
    : UiElement({rect.x, rect.y}, {rect.width, rect.height}), label_(label) {}

void Label::Update(float)
{
    text_.SetText(label_);
    text_.SetFontSize(fontSize_);
    text_.SetColor(labelColor_);
    text_.SetSpacing(fontSpacing_);
}

void Label::Draw()
{
    // TODO: add wrapping and alignment options
    auto textSize = text_.MeasureEx();
    auto center = GetCenter();
    text_.Draw(center - textSize / 2);
}
