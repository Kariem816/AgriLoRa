#pragma once

class UiBase
{
public:
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
};
