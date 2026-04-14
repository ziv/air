module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PositionSystem;

import WorldComponents;
import RenderComponents;


export void PositionSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity>();

    // get the height map to check the position can be lower than ground
    // auto playerView = registry.view<Player, Imaged>();

    for (auto [entity, position, velocity]: view.each()) {
        position.pos = position.pos + (velocity.velocity * dt);

        // todo temporary make sure not goes below ground...
        if (position.pos.y < 0) {
            position.pos.y = 0;
            if (velocity.velocity.y < 0.0f) velocity.velocity.y = 0.0f;
        }
    }
}
