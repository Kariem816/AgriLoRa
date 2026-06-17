#include "hud.h"

Hud::Hud() : Hud({0, 0}, {0, 0}) {}

Hud::Hud(const raylib::Vector2 &position, const raylib::Vector2 &size) : UiElement(position, size)
{
    btn2D_ = new Btn("2D");
    btn2D_->SetColor(raylib::Color{0xFF007ACC});
    btn2D_->SetFrame(true);
    btn2D_->SubscribeOnClick([this](raylib::Vector2 _)
                             { TriggerOnMode2D(); });

    btn3D_ = new Btn("3D");
    btn3D_->SetColor(raylib::Color{0xFF007ACC});
    btn3D_->SetFrame(true);
    btn3D_->SubscribeOnClick([this](raylib::Vector2 _)
                             { TriggerOnMode3D(); });

    AddChild("btn2D", btn2D_);
    AddChild("btn3D", btn3D_);
    updateLayout();
}

Hud::~Hud()
{
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

void Hud::updateLayout()
{
    auto pos = GetPosition();
    auto size = GetSize();

    float padding = size.x * padding_;
    raylib::Vector2 btnSize = {(size.x - padding) / 2, size.y};
    float fontSize = std::min(btnSize.x, btnSize.y) * 0.5f;

    btn2D_->SetPosition(pos);
    btn2D_->SetSize(btnSize);
    btn2D_->SetFontSize(fontSize);
    btn2D_->SetFontSpacing(fontSize / 10);

    btn3D_->SetPosition({pos.x + padding + (size.x - padding) / 2, pos.y});
    btn3D_->SetSize(btnSize);
    btn3D_->SetFontSize(fontSize);
    btn3D_->SetFontSpacing(fontSize / 10);
}