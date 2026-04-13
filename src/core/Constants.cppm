module;
#include <raymath.h>

export module Constants;

export namespace Constants {
    constexpr float G = 9.81f;
    constexpr Vector3 Gravity = {0.0f, -9.81f, 0.0f};

    constexpr Vector3 WorldForward{0.0f, 0.0f, 1.0f};
    constexpr Vector3 WorldUp{0.0f, 1.0f, 0.0f};
    constexpr Vector3 WorldRight{-1.0f, 0.0f, 0.0f};
}
