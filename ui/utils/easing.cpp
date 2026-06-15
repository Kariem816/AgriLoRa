#include "easing.h"

#include <cmath>
#include <raylib/raylib.h> // for PI

#define check(x) do { if (x < 0.0f) return 0.0f; if (x > 1.0f) return 1.0f; } while(0)

float Easing::Linear(float t) {
    check(t);
    return t;
}

float Easing::EaseInSine(float t) {
    check(t);
    return 1.0f - std::cos((t * PI) / 2.0f);
}

float Easing::EaseOutSine(float t) {
    check(t);
    return std::sin((t * PI) / 2.0f);
}

float Easing::EaseInOutSine(float t) {
    check(t);
    return -(std::cos(PI * t) - 1.0f) / 2.0f;
}
