module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PlayerPhysics;

import Types;
import Helpers;
import WorldComponents;

struct PlayerPhysicsConfig {
    Newton weight = 120000.0f; ///< Aircraft empty weight (Newtons).
    Newton engineThrust = 130000.0f; ///< Maximum engine thrust at full military power (N).
    MeterPerSecond maxSpeed = 600.0f; ///< Hard speed cap (m/s) — safety limit.
    MeterPerSecond stallSpeed = 65.0f; ///< Speed below which lift drops sharply.
    MeterPerSecond groundBrakesSpeed = 10.0f; ///< Speed threshold for ground braking damping.
    Ratio dragCoefficient = 0.36f; ///< Base aerodynamic drag coefficient.
    Ratio liftCoefficient = 1.93f;; ///< Base lift coefficient (proportional to v²).
    Ratio flyingBrakesDragRatio = 6.0f; ///< Drag multiplier when air brakes are deployed.
    Ratio flyingGearDragRatio = 1.8f; ///< Extra drag when gear is deployed in flight.
    Ratio groundBrakesDragRatio = 1000.0f; ///< Drag multiplier for wheel brakes on the ground.
    Ratio stallLiftRatio = 0.1f; ///< Lift reduction factor below stall speed.
};

export class PlayerPhysics {
    PlayerPhysicsConfig conf;

public:
    void update(entt::registry &registry, const float dt) const {
        for (const auto view = registry.view<Player, PlayerInputs>(); auto [entity, player, inputs]: view.each()) {
            const auto squareSpeed = player.speed * player.speed;

            auto cd = conf.dragCoefficient;
            if (inputs.brakes) cd += cd;
            if (inputs.gear) cd += cd;

            auto thrust = inputs.throttle * conf.engineThrust;
            auto lift = squareSpeed * conf.liftCoefficient;
            auto drag = squareSpeed * cd;

            const float mass = conf.weight / 9.81f;

            // --- Force vectors (world space) ---
            const auto thrustForce = player.forward * thrust;
            const auto dragForce = player.forward * -drag;
            const auto liftForce = player.up * lift;
            const auto weightForce = Gravity() * mass;

            const auto total = thrustForce + dragForce + weightForce + liftForce;
            const auto acceleration = total / mass;

            // --- Euler integration ---
            player.velocity = player.velocity + (acceleration * dt);
            player.speed = Vector3Length(player.velocity);

            // hard speed cap (normally drag balances thrust before this limit)
            if (player.speed > conf.maxSpeed && player.speed != 0.0f) {
                player.velocity = player.velocity * conf.maxSpeed / player.speed;
                player.speed = Vector3Length(player.velocity);
            }

            // don't mess with near zero speed
            if (player.speed < 0.2f) {
                player.velocity = Vector3Zero();
                player.speed = 0.0f;
            }

            //
            // // --- Weathervaning: align velocity toward the nose above stall speed ---
            // // https://en.wikipedia.org/wiki/Weathervane_effect
            // if (state.forces.speed > conf.stallSpeed) {
            //     auto [x, y, z] = state.orientation.forward * state.forces.speed;
            //     state.forces.velocity.x = Lerp(state.forces.velocity.x, x, 2.0f * dt);
            //     state.forces.velocity.y = Lerp(state.forces.velocity.y, y, 2.0f * dt);
            //     state.forces.velocity.z = Lerp(state.forces.velocity.z, z, 2.0f * dt);
            // }
            //
            // update position
            player.pos = player.pos + (player.velocity * dt);

            if (player.pos.y < 0.0f) {
                player.pos.y = 0.0f;
                if (player.velocity.y < 0.0f) player.velocity.y = 0.0f;
            }
            //
            // // --- ground / surface clamping ---
            // // effectiveFloorHeight is set by GameData each frame:
            // //   - over a landing zone → max(terrain, deck) + wheel clearance
            // //   - elsewhere           → terrain height + wheel clearance
            // if (state.position.y < state.effectiveFloorHeight) {
            //     state.position.y = state.effectiveFloorHeight;
            //     // cancel any remaining downward velocity so the aircraft rests on the surface
            //     if (state.forces.velocity.y < 0.0f) state.forces.velocity.y = 0.0f;
            // }
            //
            // // --- Large-world coordinate re-centering ---
            // // Shift position back toward the origin when it drifts too far,
            // // accumulating the offset in mapOffset so world-space calculations remain correct.
            // const auto ST = state.SHIFT_THRESHOLD;
            // if (state.position.x > ST) {
            //     state.position.x -= ST;
            //     state.mapOffset.x -= ST;
            // } else if (state.position.x < -ST) {
            //     state.position.x += ST;
            //     state.mapOffset.x += ST;
            // }
            // if (state.position.z > ST) {
            //     state.position.z -= ST;
            //     state.mapOffset.y -= ST;
            // } else if (state.position.z < -ST) {
            //     state.position.z += ST;
            //     state.mapOffset.y += ST;
            // }
        }
    }
};
