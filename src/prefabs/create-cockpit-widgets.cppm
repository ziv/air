module;
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../lib/ray.hpp"

export module Prefabs:CockpitWidgets;

import JsonConfig;
import Components;
import RaylibResource;
import ResourceManager;
import Types;

export namespace Factories {
    void createCockpitWidgets(entt::registry &registry,
                              const JsonConfig &config) {
        auto &manager = get_resource_manager(registry);

        // widget A
        const auto screen_a = registry.create();
        registry.emplace<DashboardSlot>(screen_a, 0);
        registry.emplace<Position2D>(screen_a, (Vector2){528.0f, 612.0});

        // add configuration
        const auto minimap_cfg = config.get<MinimapConfig>("/views/minimap");
        registry.emplace<MinimapWidget>(screen_a, minimap_cfg);

        // add texture
        if (const auto tex_id = entt::hashed_string(minimap_cfg.mapTexture.c_str()); manager.tex.contains(tex_id)) {
            registry.emplace<WithTexture>(screen_a, manager.tex[tex_id]);
        } else {
            TraceLog(LOG_WARNING, "minimap texture '%s' not found in cache", minimap_cfg.mapTexture.c_str());
        }


        // widget B
        const auto screen_b = registry.create();
        registry.emplace<DashboardSlot>(screen_b, 1);
        registry.emplace<Position2D>(screen_b, (Vector2){752.0f, 597.0});
        registry.emplace<EngineWidget>(screen_b);

        // widget C
        const auto screen_c = registry.create();
        registry.emplace<DashboardSlot>(screen_c, 3);
        registry.emplace<Position2D>(screen_c, (Vector2){300.0f, 597.0});

    }
}
