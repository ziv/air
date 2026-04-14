module;
#include <entt/entt.hpp>
#include <algorithm>

export module AircraftMechanicsSystem;

import AircraftComponents;
import WorldComponents;


export void AircraftMechanics(entt::registry &registry, const float dt) {
    auto view = registry.view<Engine, AircraftUtils, AircraftControls, Grounded>();

    for (auto [entity, engine, utils, controls, grounded]: view.each()) {
        if (controls.afterBurner) engine.throttle = 1.2f;
        else engine.throttle = std::clamp(engine.throttle + controls.throttle * dt, 0.0f, 1.2f);

        if (controls.brake) utils.brake = !utils.brake;

        if (controls.gear && !grounded.grounded) utils.gear = !utils.gear;
    }
}
