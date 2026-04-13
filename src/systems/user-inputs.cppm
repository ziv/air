module;
#include <entt/entt.hpp>
#include <raylib.h>

export module UserInputs;

import AircraftComponents;
import WorldComponents;

export void UserInputs(entt::registry &registry) {
    auto view = registry.view<AircraftControls>();

    for (auto [entity, controls]: view.each()) {
        controls.pitch = 0.0f;
        controls.roll = 0.0f;
        controls.yaw = 0.0f;
        controls.throttle = 0.0f;
        controls.brakes = false;
        controls.gear = false;

        // steering
        if (IsKeyDown(KEY_UP)) controls.pitch = -1.0f;
        if (IsKeyDown(KEY_DOWN)) controls.pitch = 1.0;
        if (IsKeyDown(KEY_LEFT)) controls.roll = -1.0;
        if (IsKeyDown(KEY_RIGHT)) controls.roll = 1.0;
        if (IsKeyDown(KEY_Q)) controls.yaw = 1.0;
        if (IsKeyDown(KEY_E)) controls.yaw = -1.0;

        // throttling
        if (IsKeyDown(KEY_MINUS)) controls.throttle = -1.0f;
        if (IsKeyDown(KEY_EQUAL)) controls.throttle = 1.0f;

        // brake
        if (IsKeyPressed(KEY_B)) controls.brakes = true;

        // gear
        if (IsKeyPressed(KEY_G)) controls.gear = false;
    }
}
