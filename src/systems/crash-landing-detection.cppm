module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Systems:CrashLandingDetection;

import Components;
import ResourceManager;

export void CrashLandingDetection(entt::registry &registry) {
    const auto viewPlayer = registry.view<Player>();
    // const auto playerEntity = viewPlayer.front();
    // if (playerEntity == entt::null) return;
    //
    // const auto [player, gh] = viewPlayer.get<Player, GroundHeight>(playerEntity);
    // if (!player.touchdown) return; // not touching down, no crash, nothing to check
    //
    // const auto playerPosition = player.pos + player.offset;
    // const auto relativeHeight = player.pos.y - gh.height;
    //
    // if (relativeHeight > 20.0f) {
    //     return; // too high to crash
    // }
    //
    // // iterate over all landable areas to check of we in one of them
    // // todo limit this only to friendly landable areas
    // auto view = registry.view<Position3D, Landable>();
    //
    // const auto distance = 2000;
    // for (auto [entity, pos, land]: view.each()) {
    //     // not even close
    //     if (Vector3Distance(playerPosition, pos.pos) > distance) continue;
    //
    //     // check if landing is ok
    //     TraceLog(LOG_DEBUG, "Player is close to a landable area, checking landing conditions...");
    //     player.crashed = false;
    // }
    // if (relativeHeight < 1.0f) {
    //     // this is probably a crash
    //     player.crashed = true;
    // }
}
