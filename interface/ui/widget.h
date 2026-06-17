#pragma once

#include <map>
#include <string>

#include <ui/ui.h>

class Widget : public virtual UiElement
{
public:
    void Update(float dt) override;
    virtual void Draw() override;
    void AddChild(const std::string &name, UiBase *child);
    UiBase *GetChild(const std::string &name) const;
    size_t ChildrenCount() const;

private:
    std::map<std::string, UiBase *> children_;
};