module;
#include "../lib/ray.hpp"

export module Helpers;

import Types;

export Vector3 WorldForward() {
    return Vector3(0.0f, 0.0f, 1.0f);
}

export Vector3 WorldUp() {
    return Vector3(0.0f, 1.0f, 0.0f);
}

export Vector3 WorldRight() {
    return Vector3(-1.0f, 0.0f, 0.0f);
}

export Vector3 Gravity() {
    return Vector3(0.0f, -9.81f, 0.0f);
}


// conversions

export Feet meter2feet(const Meter meter) {
    return meter * 3.28084f;
}

export FeetPerMinute msToFpm(const MeterPerSecond meters) {
    return meters * 196.8504f;
}

export Knot msToKnots(const MeterPerSecond meters) {
    return meters * 1.94384f;
}


// formatters

export const char *numberSuffix(const float number) {
    if (number >= 1000000.0f) {
        return TextFormat("%.1fM", number / 1000000.0f);
    }
    if (number >= 1000.0f) {
        return TextFormat("%.1fk", number / 1000.0f);
    }
    return TextFormat("%.0f", number);
}
