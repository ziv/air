module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module PlayerPrefabs;

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
    Color tintColor{};
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PilotConfig, fov, tilt);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath, tintColor);
}

entt::entity createCockpit(entt::registry &registry,
                           const JsonConfig &config) {
    const auto [texturePath, shaderPath, tintColor] = config.get<CockpitConfig>("/cockpit");
    const auto entity = registry.create();

    registry.emplace<Position2D>(entity, Vector2Zero());
    registry.emplace<ChromaRender>(entity,
                                   TextureHandle{LoadTexture(texturePath.c_str())},
                                   ShaderHandle{LoadShader(nullptr, shaderPath.c_str())},
                                   tintColor,
                                   100
    );


    return entity;
}

export namespace Factories {
    entt::entity createPlayer(entt::registry &registry,
                              const JsonConfig &config) {
        const auto pilotConfig = config.get<PilotConfig>("/player");


        const auto player = registry.create();
        registry.emplace<Player>(player);
        registry.emplace<View3D>(player, pilotConfig.fov, pilotConfig.tilt);

        const auto cockpit = createCockpit(registry, config);
        registry.emplace<ChildOf>(cockpit, player);

        return player;
    }
}
