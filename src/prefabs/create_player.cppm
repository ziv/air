module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module PlayerPrefabs;

import Constants;
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

struct AircraftConfig {
    float weight;
    float engineThrust;
    float vleSpeed;
    float liftCoefficient;
    float dragCoefficient;
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PilotConfig, fov, tilt);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath, tintColor);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AircraftConfig, weight, engineThrust, vleSpeed, liftCoefficient, dragCoefficient);
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

// entt::entity createAircraft(entt::registry &registry,
//                             const JsonConfig &config) {
// }

export namespace Factories {
    entt::entity createPlayer(entt::registry &registry,
                              const JsonConfig &config) {
        const auto pilotConfig = config.get<PilotConfig>("/player");
        const auto aircraftConfig = config.get<AircraftConfig>("/aircraft");

        const auto player = registry.create();
        registry.emplace<Player>(player);
        registry.emplace<View3D>(player, pilotConfig.fov, pilotConfig.tilt);
        registry.emplace<StickInputs>(player, 0.0f, 0.0f, 0.0f);
        registry.emplace<Position3D>(player, Vector3Zero());
        registry.emplace<Velocity>(player, Vector3Zero());
        registry.emplace<Engine>(player, aircraftConfig.engineThrust, 0.0f);
        registry.emplace<Aircraft>(player, aircraftConfig.dragCoefficient, aircraftConfig.liftCoefficient, aircraftConfig.weight);
        registry.emplace<Orientation>(player, WorldForward, WorldUp, WorldRight);
        registry.emplace<Acceleration>(player, Vector3Zero());

        const auto cockpit = createCockpit(registry, config);
        registry.emplace<ChildOf>(cockpit, player);

        return player;
    }
}
