module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module WidgetsInputs;

import Components;

export void WidgetsInputs(entt::registry &registry) {
    for (const auto minimap_view = registry.view<MinimapWidget>(); const auto [entity, minimap]: minimap_view.each()) {
        if (IsKeyPressed(KEY_Z)) minimap.zoom += 0.4f;
        if (IsKeyPressed(KEY_X)) minimap.zoom -= 0.4f;
        minimap.zoom = std::clamp(minimap.zoom, 0.2f, 5.0f);
    }

    for (const auto radar_view = registry.view<RadarWidget>(); const auto [entity, radar]: radar_view.each()) {
        if (IsKeyPressed(KEY_R)) radar.rangeIndex = (radar.rangeIndex + 1) % radar.cfg.ranges.size();
    }
}
