#pragma once

#include <string>
#include <map>

#include <ui/clickable.h>
#include <ui/label.h>

class Btn : public Clickable, public Label
{
public:
    Btn() = default;

    void Update(float dt) override;
    void Draw() override;
};
