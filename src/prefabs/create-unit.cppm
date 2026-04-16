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

        registry.emplace<Identify>(entity, def.id);

        // resources
        auto &assets = registry.ctx().get<ResourceManager>();

        if (!def.modelId.empty()) {
            const auto modelHash = entt::hashed_string(def.modelId.data());
            // ensure model is loaded
            if (!assets.models.contains(modelHash)) {
                assets.models.load(modelHash, LoadModel(def.modelId.c_str()));
            }
            // connect model to component
            registry.emplace<Modeled>(entity, assets.models[modelHash]);
            TraceLog(LOG_DEBUG, "Entity %s has a model", def.id.c_str());
        }

        // location & orientation
        registry.emplace<Position3D>(entity, def.position);
        registry.emplace<Heading>(entity, def.heading);

        switch (def.type) {
            case EntityType::CARRIER:
                registry.emplace<Landable>(entity, true);
                registry.emplace<Carrier>(entity);
                break;
            case EntityType::AIRBASE:
                registry.emplace<Landable>(entity, false);
                break;
            default:
                break;
        }

        TraceLog(LOG_DEBUG, "Entity %s loaded", def.id.c_str());
        return entity;
    }
}
