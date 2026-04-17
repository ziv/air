module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module WorldStreamerSystem;

import Components;

export void WorldStreamerSystem(entt::registry &registry) {
    // is called streamer cause it original use was to stream
    // the world tiles

    // we are rendering the map at the offset of the player
    const auto &offset = registry.ctx().get<Offset>().offset;

    auto view = registry.view<World, Position3D, WithModel>();

    for (auto [entity, pos, mod]: view.each()) {
        DrawModel(mod.handle->res, offset, 1.0f, WHITE);
    }
}
