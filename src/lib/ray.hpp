#pragma once
// this file is because when both needed, they have the same definsions
#include <raylib.h>
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <nlohmann/json.hpp>

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Color, r, g, b, a)