module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module VelocitySystem;

import Components;

export void VelocitySystem(entt::registry &registry, float dt) {
    auto view = registry.view<LinearVelocity, LinerAcceleration, Grounded>();

    for (auto [entity, velocity, acceleration, grounded]: view.each()) {
        constexpr float maxSpeed = 600.0f;
        velocity.velocity = velocity.velocity + (acceleration.acc * dt);
        velocity.speed = Vector3Length(velocity.velocity);

        // hard speed cap (normally drag balances thrust before this limit)
        if (velocity.speed > maxSpeed && velocity.speed != 0.0f) {
            velocity.velocity = velocity.velocity * maxSpeed / velocity.speed;
            velocity.speed = Vector3Length(velocity.velocity);
        }

        // on ground there is no more velocity down
        if (grounded.grounded && velocity.velocity.y < 0.0f) {
            velocity.velocity.y = 0.0f;
            velocity.speed = Vector3Length(velocity.velocity);
        }
    }
}
