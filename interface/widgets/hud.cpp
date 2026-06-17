#include "hud.h"

Hud::Hud() : Hud({0, 0}, {0, 0}) {}

Hud::Hud(const raylib::Vector2 &position, const raylib::Vector2 &size) : UiElement(position, size)
{
    btn2D_ = new Btn();
    btn2D_->SetLabel("2D");
    btn2D_->SetBackground(raylib::Color{0xFF007ACC});
    btn2D_->SetFrameThickness(2.0f);
    btn2D_->SubscribeOnClick([this](raylib::Vector2)
                             { TriggerOnMode2D(); });

    btn3D_ = new Btn();
    btn3D_->SetLabel("3D");
    btn3D_->SetBackground(raylib::Color{0xFF007ACC});
    btn3D_->SetFrameThickness(2.0f);
    btn3D_->SubscribeOnClick([this](raylib::Vector2)
                             { TriggerOnMode3D(); });

    chkDebug_ = new Checkbox();
    chkDebug_->SetBackground({0, 0, 0, 0});
    chkDebug_->SetCheckColor(raylib::Color{0xFF007ACC});
    chkDebug_->SetLabel("Debug");
    chkDebug_->SetLabelColor(BLACK);
    chkDebug_->SetFrameThickness(0.0f);

    AddChild("btn2D", btn2D_);
    AddChild("btn3D", btn3D_);
    AddChild("chkDebug", chkDebug_);
    updateLayout();
}

Hud::~Hud()
{
    delete btn2D_;
    delete btn3D_;
    delete chkDebug_;
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

bool Hud::GetDebug() const
{
    return chkDebug_->GetChecked();
}

void Hud::SetDebug(bool value)
{
    chkDebug_->SetChecked(value);
}

uint64_t Hud::SubscribeOnDebugToggle(const EventSource<bool>::Cb &callback)
{
    return chkDebug_->SubscribeOnCheck(callback);
}

void Hud::UnsubscribeOnDebugToggle(uint64_t id)
{
    return chkDebug_->UnsubscribeOnCheck(id);
}

void Hud::updateLayout()
{
    auto pos = GetPosition();
    auto size = GetSize();

    float gap = size.x * gap_;
    size_t children = ChildrenCount();
    float usableWidth = size.x - gap * (children - 1);
    raylib::Vector2 btnSize = {usableWidth / 4, size.y};
    raylib::Vector2 chkSize = {usableWidth / 2, size.y};
    float fontSize = std::min(usableWidth / 3, size.y) * 0.65;

    btn2D_->SetPosition(pos);
    btn2D_->SetSize(btnSize);
    btn2D_->SetFontSize(fontSize);
    btn2D_->SetFontSpacing(fontSize * 0.1);

    btn3D_->SetPosition({pos.x + btnSize.x + gap, pos.y});
    btn3D_->SetSize(btnSize);
    btn3D_->SetFontSize(fontSize);
    btn3D_->SetFontSpacing(fontSize * 0.1);

    chkDebug_->SetPosition({pos.x + 2 * (btnSize.x + gap), pos.y});
    chkDebug_->SetSize(chkSize);
    chkDebug_->SetFontSize(fontSize);
    chkDebug_->SetFontSpacing(fontSize * 0.1);
}