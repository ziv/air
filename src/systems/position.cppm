module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PositionSystem;

import AircraftComponents;
import WorldComponents;


export void PositionSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity>();

    for (auto [entity, position, velocity]: view.each()) {
        position.pos = position.pos + (velocity.velocity * dt);
    }
}
