#pragma once

#include <string>
#include <map>

#include <ui/clickable.h>
#include <ui/label.h>
#include <ui/drawable.h>

class Btn : public Clickable, public Label, public Drawable
{
public:
    Btn() = default;
    virtual ~Btn() = default;

    void Update(float dt) override;
    void Draw() override;
};
