#pragma once

#include <ui/widget.h>
#include <ui/btn.h>

class Hud : public Widget
{
public:
    Hud();
    Hud(const raylib::Vector2 &position, const raylib::Vector2 &size);
    ~Hud();

    void SetPosition(raylib::Vector2 position) override;
    void SetSize(raylib::Vector2 size) override;

    EVENTSOURCEV(mode2DEv, OnMode2D)
    EVENTSOURCEV(mode3DEv, OnMode3D)

private:
    void updateLayout();

public:
    static constexpr float padding_ = 0.01f;

private:
    Btn *btn2D_;
    Btn *btn3D_;
};
