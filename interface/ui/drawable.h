#pragma once

#include <ui/ui.h>

class Drawable : public virtual UiElement
{    
public:
    virtual void Draw() override;
    
    VGETTERSETTER(raylib::Color, Background, bg_)
    VGETTERSETTER(raylib::Color, FrameColor, frameColor_)
    VGETTERSETTER(float, FrameThickness, frameThickness_)
    
private:
    raylib::Color bg_{0x80181818};
    float frameThickness_ = 2.0f;
    raylib::Color frameColor_{BLACK};
};