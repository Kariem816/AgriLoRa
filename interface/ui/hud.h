#pragma once

#include <ui/ui.h>
#include <ui/btn.h>

class Hud : public UiElement
{
public:
    Hud();
    Hud(const raylib::Vector2 &position, const raylib::Vector2 &size);
    ~Hud();

    void SetPosition(raylib::Vector2 position) override;
    void SetSize(raylib::Vector2 size) override;

private:
    void on2DMode();
    void on3DMode();
    void updateLayout();

public:
    static constexpr float padding_ = 10.0f;
    Btn* btn2D_;
    Btn* btn3D_;
};
