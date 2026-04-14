module;
#include "../lib/ray.hpp"

export module Helpers;

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