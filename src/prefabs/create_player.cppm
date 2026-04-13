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
import ConfigTypes;

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
        const auto aircraftConfig = config.get<AircraftConfig>("/aircraft");

        const auto player = registry.create();
        registry.emplace<Player>(player);
        registry.emplace<View3D>(player, pilotConfig.fov, pilotConfig.tilt);
        registry.emplace<AircraftControls>(player, 0.0f, 0.0f, 0.0f, false, false, 0.0f);
        registry.emplace<Position3D>(player, Vector3Zero());
        registry.emplace<Velocity>(player, Vector3Zero());
        registry.emplace<Engine>(player, aircraftConfig.engineThrust, 0.0f);
        registry.emplace<Orientation>(player, Constants::WorldForward, Constants::WorldUp, Constants::WorldRight);
        registry.emplace<Acceleration>(player, Vector3Zero());
        registry.emplace<AngularAcceleration>(player, Vector3Zero());
        registry.emplace<AngularVelocity>(player, Vector3Zero());
        registry.emplace<Rotation>(player, QuaternionIdentity());
        registry.emplace<AircraftUtils>(player, true, true);
        registry.emplace<Aircraft>(player,
                                   aircraftConfig.dragCoefficient,
                                   aircraftConfig.inducedDragCoefficient,
                                   aircraftConfig.liftCoefficient,
                                   aircraftConfig.liftSlopeCoefficient,
                                   aircraftConfig.stallAngle,
                                   aircraftConfig.weight,
                                   aircraftConfig.pitchRatio,
                                   aircraftConfig.rollRatio,
                                   aircraftConfig.yawRatio);


        const auto cockpit = createCockpit(registry, config);
        registry.emplace<ChildOf>(cockpit, player);

        return player;
    }
}
