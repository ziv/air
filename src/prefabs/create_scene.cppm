module;
#include <entt/entt.hpp>
#include <raymath.h>

export module ScenePrefabs;

import JsonConfig;
import WorldComponents;
import AircraftComponents;
import RaylibResource;
import ConfigTypes;


export namespace Factories {
    entt::entity createScene(entt::registry &registry,
                             const JsonConfig &config) {
        // const auto conf = config.get<SceneConfig>("/scene");
        const auto entity = registry.create();

        registry.emplace<World>(entity);
        registry.emplace<Position3D>(entity, Vector3Zero());


        return entity;
    }
}
