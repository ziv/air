module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module Prefabs:Cockpit;

import JsonConfig;
import WorldComponents;
import AircraftComponents;
import RaylibResource;
import Types;

export namespace Factories {
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
}
