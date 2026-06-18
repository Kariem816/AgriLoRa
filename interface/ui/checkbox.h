#pragma once

#include <ui/clickable.h>
#include <ui/drawable.h>
#include <ui/label.h>
#include <ui/macros.h>

class Checkbox : public Clickable, public Drawable, public Label
{
public:
    Checkbox();
    Checkbox(const raylib::Vector2 &position, const raylib::Vector2 &size, const std::string &label = "");
    virtual ~Checkbox() = default;

    void Update(float dt) override;
    void Draw() override;

    GETTERSETTER(bool, Checked, checked_)
    VGETTERSETTER(raylib::Color, CheckBackground, checkBg_)
    VGETTERSETTER(raylib::Color, CheckColor, checkColor_)
    VGETTER(float, LabelSize, labelSize_)
    VGETTER(float, Gap, gap_)

    void SetPosition(raylib::Vector2 position) override;
    void SetSize(raylib::Vector2 size) override;

    virtual void SetLabelSize(float value);
    virtual void SetGap(float value);

    EVENTSOURCE(checkEv, OnCheck, bool)

private:
    void updateLayout();

private:
    raylib::Rectangle checkboxRect_;
    raylib::Rectangle labelRect_;

    bool checked_ = false;
    raylib::Color checkBg_ = WHITE;
    raylib::Color checkColor_ = BLACK;
    float labelSize_ = 0.75f;
    float gap_ = 0.05f;

    const float checkboxPadding_ = 0.1f;
};