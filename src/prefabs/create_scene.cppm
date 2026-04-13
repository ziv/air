module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module ScenePrefabs;

import JsonConfig;
import WorldComponents;
import AircraftComponents;
import RaylibResource;

struct PilotConfig {
    float fov;
    float tilt;
};

struct CockpitConfig {
    std::string texturePath;
    std::string shaderPath;
};

struct SceneConfig {
    Color skyColor;
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PilotConfig, fov, tilt);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SceneConfig, skyColor);
}


export namespace Factories {
    entt::entity createScene(entt::registry &registry,
                             const JsonConfig &config) {
        const auto conf = config.get<SceneConfig>("/scene");
        const auto entity = registry.create();

        registry.emplace<World>(entity);
        registry.emplace<Position3D>(entity, Vector3Zero());


        return entity;
    }
}
