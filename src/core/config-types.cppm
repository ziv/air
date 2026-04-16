module;
#include <nlohmann/json.hpp>
#include <string>
#include "../lib/ray.hpp"

export module Types:Config;

import :Units;

export struct GlobalConfig {
    // window
    std::string title;
    Pixel width;
    Pixel height;

    // player tilt down and field of view
    AngleRad tilt;
    AngleDeg fov;

    // where to clip planes
    Meter nearPlane;
    Meter farPlane;
};

export struct WindowConfig {
    std::string title;
    Pixel width;
    Pixel height;
    Meter nearPlane;
    Meter farPlane;
};

export struct SceneConfig {
    std::string mapTexture;
    std::string mapHeightmap;
    Vector3 mapSize;
};

export struct CockpitConfig {
    std::string texturePath;
    std::string shaderPath;
    Color tintColor{};
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GlobalConfig, title, width, height, tilt, fov, nearPlane, farPlane);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowConfig, title, width, height, nearPlane, farPlane);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SceneConfig, mapTexture, mapHeightmap, mapSize);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath, tintColor);
}
