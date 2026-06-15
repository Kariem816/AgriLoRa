#pragma once

class Easing {
public:
    static float Linear(float t);
    static float EaseInSine(float t);
    static float EaseOutSine(float t);
    static float EaseInOutSine(float t);
};
