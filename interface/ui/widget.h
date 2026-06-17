#pragma once

#include <map>
#include <string>

#include <ui/ui.h>

class Widget : public virtual UiElement
{
public:
    void Update(float dt) override;
    void Draw() override;
    void AddChild(const std::string &name, UiElement *child);
    UiElement *GetChild(const std::string &name) const;

private:
    std::map<std::string, UiElement *> children_;
};