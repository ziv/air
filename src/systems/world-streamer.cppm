module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module WorldStreamerSystem;

import WorldComponents;
import RenderComponents;

export void WorldStreamerSystem(entt::registry &registry) {
    const auto viewPlayer = registry.view<Player>();
    const auto playerEntity = viewPlayer.front();

    if (playerEntity == entt::null) return;

    // we are rendering the map at the offset of the player
    const auto &offset = viewPlayer.get<Player>(playerEntity).offset;

    auto view = registry.view<Position3D, Modeled, World>();

    for (auto [entity, position, modeled]: view.each()) {
        DrawModel(modeled.handle->model, offset, 1.0f, WHITE);
    }
}
