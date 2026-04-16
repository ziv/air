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
    void createCockpitWidgets(entt::registry &registry,
                               const JsonConfig &config) {
        // create screens containers and set defaults widgets
        const auto screen_a = registry.create();
        // todo take those from conf or consts?!
        registry.emplace<Position2D>(screen_a, (Vector2){528.0f, 612.0});
        registry.emplace<MinimapWidget>(screen_a);
    }
}
