#include "window.h"

#include <algorithm>

Window &Window::Instance() {
    static Window instance;
    return instance;
}

void Window::Update(float dt) {
    for (auto &widget : widgets_) {
        widget->Update(dt);
    }
}

void Window::Draw() {
    for (const auto &widget : widgets_) {
        widget->Draw();
    }
}

void Window::AddWidget(Widget *widget) {
    widgets_.push_back(widget);
}

bool Window::RemoveWidget(Widget *widget) {
    auto it = std::find(widgets_.begin(), widgets_.end(), widget);
    if (it != widgets_.end()) {
        widgets_.erase(it);
        return true;
    }
    return false;
}
