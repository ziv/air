module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Prefabs:Unit;

import JsonConfig;
import Types;
import ResourceManager;
import Components;

export namespace Factories {
    entt::entity createUnit(entt::registry &registry,
                            const EntityDef &def) {
        const auto entity = registry.create();

        // resources
        auto &assets = registry.ctx().get<ResourceManager>();

        if (!def.modelId.empty()) {
            // ensure model is loaded
            if (const auto modelHash = entt::hashed_string(def.modelId.data()); !assets.models.contains(modelHash)) {
                const auto model = LoadModel(def.modelId.c_str());
                assets.models.load(modelHash, model);
                // connect model to component
                registry.emplace<Modeled>(entity, assets.models[modelHash]);
            }
        }

        // location
        registry.emplace<Position3D>(entity, def.position);

        return entity;
    }
}
