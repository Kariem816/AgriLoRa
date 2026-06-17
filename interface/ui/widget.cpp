#include "widget.h"

void Widget::Update(float dt)
{
    for (auto &child : children_)
    {
        child.second->Update(dt);
    }
}

void Widget::Draw()
{
    for (const auto &child : children_)
    {
        child.second->Draw();
    }
}

void Widget::AddChild(const std::string &name, UiElement *child)
{
    children_[name] = child;
}

UiElement *Widget::GetChild(const std::string &name) const
{
    auto it = children_.find(name);
    if (it != children_.end())
    {
        return it->second;
    }
    return nullptr;
}