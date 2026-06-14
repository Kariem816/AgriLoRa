#pragma once

#include <cmath>
#include <raylib/raylib.h>

#define check(x) do { if (x < 0.0f) return 0.0f; if (x > 1.0f) return 1.0f; } while(0)

namespace Easing {
    float Linear(float t) {
        check(t);
        return t;
    }
    float EaseInSine(float t) {
        check(t);
        return 1.0f - cosf((t * PI) / 2.0f);
    }
    float EaseOutSine(float t) {
        check(t);
        return sinf((t * PI) / 2.0f);
    }
    float EaseInOutSine(float t) {
        check(t);
        return -(cosf(PI * t) - 1.0f) / 2.0f;
    }
};
