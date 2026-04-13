module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module MovementSystem;

import AircraftComponents;
import WorldComponents;


export void MovementSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity>();

    for (auto [entity, position, velocity]: view.each()) {

    }
    // for (auto entity: view) {
    //     auto &position = view.get<Position3D>(entity);
    //     const auto &velocity = view.get<Velocity>(entity);
    //
    //     position.pos = Vector3Add(position.pos, Vector3Scale(velocity.velocity, dt));
    // }
}
