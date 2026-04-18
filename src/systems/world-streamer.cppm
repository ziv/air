module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include "rlgl.h"

export module WorldStreamerSystem;

import Components;

export void WorldStreamerSystem(entt::registry &registry) {
    // is called streamer cause it original use was to stream
    // the world tiles

    // we are rendering the map at the offset of the player
    const auto &offset = registry.ctx().get<Offset>().offset;

    for (const auto view = registry.view<World, Position3D>(); auto [entity, world, pos]: view.each()) {
        DrawModel(world.surface->res, offset, 1.0f, WHITE);


        // todo move this line to an update world system
        // world.clouds->res.materials[0].maps[MATERIAL_MAP_DIFFUSE].color.r += 0.01f * GetFrameTime();
        const Vector3 cloudPosition = {
            offset.x,
            10000.0f,
            offset.y
        };
        rlDisableBackfaceCulling();
        DrawModel(world.clouds->res, cloudPosition, 1.0f, WHITE);
        rlEnableBackfaceCulling();
    }
}
