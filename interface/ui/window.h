#pragma once

#include <vector>

#include <ui/widget.h>

class Window
{
private:
    Window() = default;

public:
    static Window &Instance();
    void Update(float dt);
    void Draw();
    void AddWidget(Widget *widget);
    bool RemoveWidget(Widget *widget);

private:
    std::vector<Widget *> widgets_;
};
