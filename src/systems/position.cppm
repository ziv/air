module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PositionSystem;

import Components;

export void PositionSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, LinearVelocity, Grounded, Player>();

    // get the height map to check the position can be lower than ground
    // auto playerView = registry.view<Player, Imaged>();

    for (auto [entity, position, velocity, grounded, player]: view.each()) {
        constexpr float THRESHOLD = 5000.0f;
        position.pos = position.pos + (velocity.velocity * dt);

        // todo temporary make sure not goes below ground...should be check against the height map
        if (position.pos.y < 3) {
            position.pos.y = 3;
            grounded.grounded = true;
        }

        if (position.pos.y > 3) {
            grounded.grounded = false;
        }

        // --- Large-world coordinate re-centering ---
        // Shift position back toward the origin when it drifts too far,
        // accumulating the offset so world-space calculations remain correct.
        if (position.pos.x > THRESHOLD) {
            position.pos.x -= THRESHOLD;
            position.offset.x -= THRESHOLD;
        } else if (position.pos.x < -THRESHOLD) {
            position.pos.x += THRESHOLD;
            position.offset.x += THRESHOLD;
        }
        if (position.pos.z > THRESHOLD) {
            position.pos.z -= THRESHOLD;
            position.offset.z -= THRESHOLD;
        } else if (position.pos.z < -THRESHOLD) {
            position.pos.z += THRESHOLD;
            position.offset.z += THRESHOLD;
        }
    }
}
