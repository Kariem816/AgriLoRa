#pragma once

#include <algorithm>
#include <functional>

#include "easing.h"

using EasingFn = std::function<float(float)>;

enum class LoopMode
{
    None,
    Repeat,
    Reverse
};

template <typename T>
class Animatable
{
public:
    virtual ~Animatable() = default;
    // Should be called every frame with the time elapsed since the last frame
    virtual void update(float deltaTime) = 0;
    // Returns the current value of the animatable property
    virtual T value() const = 0;
    // Instantly sets the value to the target without animation. disables looping if it was enabled.
    virtual void to(T target) = 0;
    // Starts an animation towards the target value over the specified duration with easing
    virtual void to(T target, float duration, EasingFn easing) = 0;
    virtual void to(T target, float duration)
    {
        to(target, duration, Easing::Linear);
    }
    // Gets whether the animation is currently active (i.e., if it's still animating towards the target)
    virtual bool isAnimating() const = 0;
    // Sets the animation to loop
    virtual void setLoop(LoopMode loop) = 0;
};

class AnimatableFloat : public Animatable<float>
{
private:
    float _value;
    float initialValue;
    float target;
    float duration;
    float elapsed;
    EasingFn easing;
    LoopMode loop;

public:
    using Animatable<float>::to; // Bring the base class to() into scope
    AnimatableFloat(float value)
        : _value(value), initialValue(value), target(value), duration(0.0f), elapsed(0.0f) {}

    void update(float deltaTime) override
    {
        if (elapsed < duration)
        {
            elapsed += deltaTime;
            float t = easing(elapsed / duration);
            _value = initialValue + t * (target - initialValue);
        }
        else if (loop == LoopMode::Repeat)
        {
            elapsed = 0.0f;
        }
        else if (loop == LoopMode::Reverse)
        {
            to(initialValue, duration, easing);
        }
        else
        {
            _value = target;
        }
    }

    float value() const override
    {
        return _value;
    }

    void to(float target) override
    {
        _value = target;
        elapsed = 0.0f;
        duration = 0.0f;
        loop = LoopMode::None;
    }

    void to(float target, float duration, EasingFn easing) override
    {
        this->initialValue = _value;
        this->target = target;
        this->duration = duration;
        this->elapsed = 0.0f;
        this->easing = easing;
    }

    bool isAnimating() const override
    {
        return elapsed < duration;
    }

    void setLoop(LoopMode loop) override
    {
        this->loop = loop;
    }
};