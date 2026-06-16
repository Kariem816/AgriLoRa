#include "hud.h"

#include <iostream>

Hud::Hud() : Hud({0, 0}, {0, 0}) {}

Hud::Hud(const raylib::Vector2 &position, const raylib::Vector2 &size) : UiElement(position, size)
{
    btn2D_ = new Btn("2D");
    btn2D_->SetColor(raylib::Color{0xFF007ACC});
    btn2D_->SetFrame(true);
    btn2D_->SubscribeOnClick([this](raylib::Vector2 _) { on2DMode(); });

    btn3D_ = new Btn("3D");
    btn3D_->SetColor(raylib::Color{0xFF007ACC});
    btn3D_->SetFrame(true);
    btn3D_->SubscribeOnClick([this](raylib::Vector2 _) { on3DMode(); });


    AddChild("btn2D", btn2D_);
    AddChild("btn3D", btn3D_);
    updateLayout();
}

Hud::~Hud() {
    delete btn2D_;
    delete btn3D_;
}

void Hud::SetPosition(raylib::Vector2 position)
{
    UiElement::SetPosition(position);
    updateLayout();
}

void Hud::SetSize(raylib::Vector2 size)
{
    UiElement::SetSize(size);
    updateLayout();
}

void Hud::on2DMode() {
    std::cout << "Switched to 2D mode" << std::endl;
}

void Hud::on3DMode() {
    std::cout << "Switched to 3D mode" << std::endl;
}


void Hud::updateLayout()
{
    auto pos = GetPosition();
    auto size = GetSize();

    btn2D_->SetPosition(pos);
    btn2D_->SetSize({(size.x - padding_) / 2, size.y});
    btn3D_->SetPosition({pos.x + padding_ + (size.x - padding_) / 2, pos.y});
    btn3D_->SetSize({(size.x - padding_) / 2, size.y});
}