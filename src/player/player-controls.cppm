module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PlayerControls;

import WorldComponents;

export class PlayerControls {
    float pitchRatio = 0.7f; ///< Pitch sensitivity (degrees per second at full deflection).
    float rollRatio = 0.9f; ///< Roll sensitivity.
    float yawRatio = 0.5f; ///< Yaw (rudder) sensitivity.

public:
    void update(entt::registry &registry, const float dt) const {
        for (const auto view = registry.view<Player, PlayerInputs>(); auto [entity, player, inputs]: view.each()) {
            inputs.pitch = 0.0f;
            inputs.roll = 0.0f;
            inputs.yaw = 0.0f;

            if (IsKeyDown(KEY_UP)) inputs.pitch = -pitchRatio * dt;
            if (IsKeyDown(KEY_DOWN)) inputs.pitch = pitchRatio * dt;

            if (IsKeyDown(KEY_LEFT)) inputs.roll = -rollRatio * dt;
            if (IsKeyDown(KEY_RIGHT)) inputs.roll = rollRatio * dt;

            if (IsKeyDown(KEY_Q)) inputs.yaw = yawRatio * dt;
            if (IsKeyDown(KEY_E)) inputs.yaw = -yawRatio * dt;

            if (IsKeyDown(KEY_MINUS)) inputs.throttle -= 0.2f * dt;
            if (IsKeyDown(KEY_EQUAL)) inputs.throttle += 0.2f * dt;

            if (IsKeyDown(KEY_A)) inputs.throttle = 1.2f;

            inputs.throttle = std::clamp(inputs.throttle, 0.0f, 1.2f);

            if (IsKeyPressed(KEY_B)) inputs.brakes = !inputs.brakes;

            // don't allow closing gear when on ground
            // todo this is temporary check, replace with flying
            if (IsKeyPressed(KEY_G) && player.pos.y > 10.0f) inputs.gear = !inputs.gear;
        }
    }
};
