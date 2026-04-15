module;
#include <iostream>
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Player:Position;

import Components;
import Types;
import Helpers;


export class PlayerPosition {
    float THRESHOLD = 5000.0f;
public:
    void update(entt::registry &registry, const float dt) const {
        for (const auto view = registry.view<Player>(); auto [entity, player]: view.each()) {
            // update position
            player.pos = player.pos + (player.velocity * dt);

            if (player.pos.y < 8.0f) {
                player.pos.y = 8.0f;
                if (player.velocity.y < 0.0f) player.velocity.y = 0.0f;
            }

            if (player.pos.x > THRESHOLD) {
                std::cout << "???" << player.pos.x << std::endl;
                player.pos.x -= THRESHOLD;
                player.offset.x -= THRESHOLD;
            } else if (player.pos.x < -THRESHOLD) {
                player.pos.x += THRESHOLD;
                player.offset.x += THRESHOLD;
            }
            if (player.pos.z > THRESHOLD) {
                player.pos.z -= THRESHOLD;
                player.offset.z -= THRESHOLD;
            } else if (player.pos.z < -THRESHOLD) {
                player.pos.z += THRESHOLD;
                player.offset.z += THRESHOLD;
            }
            // keep offset globally
            registry.ctx().insert_or_assign<Offset>(Offset{player.offset});
        }
    }
};
