module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

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

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PilotConfig, fov, tilt);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath);
}


export namespace Factories {
    entt::entity createScene(entt::registry &registry,
                             const JsonConfig &config) {
        const auto pilotConfig = config.get<PilotConfig>("/player");


        const auto entity = registry.create();
        // registry.emplace<Player>(player);
        // registry.emplace<View3D>(player, pilotConfig.fov, pilotConfig.tilt);
        //
        // const auto cockpit = createCockpit(registry, config);
        // registry.emplace<ChildOf>(cockpit, player);

        return entity;
    }
}
