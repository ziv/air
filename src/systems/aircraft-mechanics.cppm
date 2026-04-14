module;
#include <entt/entt.hpp>
#include <algorithm>

export module AircraftMechanicsSystem;

import AircraftComponents;
import WorldComponents;


export void AircraftMechanics(entt::registry &registry, float dt) {
    auto view = registry.view<Engine, AircraftUtils, AircraftControls>();

    for (auto [entity, engine, utils, controls]: view.each()) {
        engine.throttle = std::clamp(engine.throttle + controls.throttle * dt, 0.0f, 1.2f);

        if (controls.brake) utils.brake = !utils.brake;

        // todo this is temporary, should add check if its flying....
        if (controls.gear) utils.gear = !utils.gear;
    }
}
