#pragma once

#include <algorithm>
#include <functional>

#include <raylib-cpp/raylib-cpp.hpp>
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
    Animatable();
    Animatable(T val);
    Animatable(const Animatable<T>&) = default;

    virtual ~Animatable() = default;
    // Should be called every frame with the time elapsed since the last frame
    virtual void update(float deltaTime);
    // Returns the current value of the animatable property
    virtual T value() const;
    // Instantly sets the value to the target without animation. disables looping if it was enabled.
    virtual void to(T target);
    // Starts an animation towards the target value over the specified duration with easing
    virtual void to(T target, float duration, EasingFn easing);
    virtual void to(T target, float duration)
    {
        to(target, duration, Easing::Linear);
    }
    // Gets whether the animation is currently active (i.e., if it's still animating towards the target)
    virtual bool isAnimating() const;
    // Sets the animation to loop
    virtual void setLoop(LoopMode loop);

    virtual Animatable<T> &operator=(T target)
    {
        to(target);
        return *this;
    }

    virtual Animatable<T> &operator+=(T delta)
    {
        to(value() + delta);
        return *this;
    }

    virtual Animatable<T> &operator-=(T delta)
    {
        to(value() - delta);
        return *this;
    }

    virtual Animatable<T> &operator*=(T factor)
    {
        to(value() * factor);
        return *this;
    }

    virtual Animatable<T> &operator/=(T divisor)
    {
        to(value() / divisor);
        return *this;
    }

    virtual operator T() const
    {
        return value();
    }

private:
    T val;
    T initialValue;
    T target;
    float duration;
    float elapsed;
    EasingFn easing;
    LoopMode loop;
};

// Template implementations
template <typename T>
Animatable<T>::Animatable()
    : val(T()), initialValue(T()), target(T()), duration(0.0f), elapsed(0.0f) {}

template <typename T>
Animatable<T>::Animatable(T val)
    : val(val), initialValue(val), target(val), duration(0.0f), elapsed(0.0f) {}

template <typename T>
void Animatable<T>::update(float deltaTime)
{
    if (duration <= 0.0f)
    {
        val = target;
        return;
    }

    if (elapsed >= duration && loop == LoopMode::None)
    {
        val = target;
        return;
    }

    elapsed += deltaTime;

    while (elapsed >= duration)
    {
        if (loop == LoopMode::Repeat)
        {
            elapsed -= duration;
        }
        else if (loop == LoopMode::Reverse)
        {
            elapsed -= duration;
            std::swap(initialValue, target);
        }
        else // LoopMode::None
        {
            elapsed = duration;
            break;
        }
    }

    float t = easing(elapsed / duration);
    val = initialValue + (target - initialValue) * t;
}

template <typename T>
T Animatable<T>::value() const
{
    return val;
}

template <typename T>
void Animatable<T>::to(T target)
{
    initialValue = target;
    this->target = target;
    val = target;

    elapsed = 0.0f;
    duration = 0.0f;
    loop = LoopMode::None;
}

template <typename T>
void Animatable<T>::to(T target, float duration, EasingFn easing)
{
    this->initialValue = val;
    this->target = target;
    this->duration = duration;
    this->elapsed = 0.0f;
    this->easing = easing;
}

template <typename T>
bool Animatable<T>::isAnimating() const
{
    return elapsed < duration;
}

template <typename T>
void Animatable<T>::setLoop(LoopMode loop)
{
    this->loop = loop;
}
