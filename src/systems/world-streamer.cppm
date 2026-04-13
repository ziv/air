module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module WorldStreamerSystem;

import AircraftComponents;
import WorldComponents;

bool IsTileLoaded(entt::registry &registry, int x, int z) {
    auto view = registry.view<Tile>();

    for (auto entity: view) {
        const auto &tile = view.get<Tile>(entity);
        if (tile.x == x && tile.z == z) {
            return true;
        }
    }
    return false;
}

export void WorldStreamerSystem(entt::registry &registry) {
    auto viewPlayer = registry.view<Player, Position3D>();
    auto playerEntity = viewPlayer.front();

    if (playerEntity == entt::null) return;

    const auto &playerPos = viewPlayer.get<Position3D>(playerEntity).pos;

    const float TILE_SIZE = 1000.0f;
    int currentX = static_cast<int>(floor(playerPos.x / TILE_SIZE));
    int currentY = static_cast<int>(floor(playerPos.z / TILE_SIZE));

    // load required
    for (int x = currentX - 1; x <= currentX + 1; ++x) {
        for (int y = currentY - 1; y <= currentY + 1; ++y) {
            // if (!IsTileLoaded(registry, x, y)) {
            // LoadTile(registry, x, y);
            // }
        }
    }

    // delete all that not in range
    auto view = registry.view<Tile>();
    for (auto entity: view) {
        auto &tile = view.get<Tile>(entity);
        if (abs(tile.x - currentX) > 1 || abs(tile.z - currentY) > 1) {
            registry.destroy(entity);
        }
    }

    // todo update shift
}
