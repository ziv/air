module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module VelocitySystem;

import WorldComponents;


export void VelocitySystem(entt::registry &registry, float dt) {
    auto view = registry.view<Velocity, Acceleration>();

    for (auto [entity, velocity, acceleration]: view.each()) {
        velocity.velocity = velocity.velocity + (acceleration.linear * dt);
    }
}
