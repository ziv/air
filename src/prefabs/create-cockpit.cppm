module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module Prefabs:Cockpit;

import JsonConfig;
import Components;
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

        // create screens containers and set defaults widgets
        const auto screen_a = registry.create();
        registry.emplace<Position2D>(screen_a, (Vector2){528.0f, 612.0});
        registry.emplace<MinimapWidget>(screen_a);

        return entity;
    }
}
