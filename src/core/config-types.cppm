module;
#include <nlohmann/json.hpp>
#include <string>
#include "../lib/ray.hpp"

export module Types:Config;

export struct WindowConfig {
    std::string title;
    int width;
    int height;
    float nearPlane;
    float farPlane;
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
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowConfig, title, width, height, nearPlane, farPlane);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SceneConfig, mapTexture, mapHeightmap, mapSize);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath, tintColor);
}
