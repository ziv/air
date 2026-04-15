module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Player:Physics;

import Types;
import Helpers;
import WorldComponents;
import :Config;


export class PlayerPhysics {
    PlayerPhysicsConfig conf;

public:
    explicit PlayerPhysics(const PlayerPhysicsConfig &c) : conf(c) {
    }

    void update(entt::registry &registry, const float dt) const {
        for (const auto view = registry.view<Player, PlayerInputs>(); auto [entity, player, inputs]: view.each()) {
            const auto squareSpeed = player.speed * player.speed;

            auto cd = conf.dragCoefficient;
            if (inputs.brakes) cd += cd;
            if (inputs.gear) cd += cd;

            auto drag = squareSpeed * cd;
            const auto thrust = inputs.throttle * conf.engineThrust;
            const auto lift = squareSpeed * conf.liftCoefficient;

            // there is no such thing a negative drag
            if (drag > thrust) drag = thrust;

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


            // --- Weathervaning: align velocity toward the nose above stall speed ---
            // https://en.wikipedia.org/wiki/Weathervane_effect
            if (player.speed > conf.stallSpeed) {
                auto [x, y, z] = player.forward * player.speed;
                player.velocity.x = Lerp(player.velocity.x, x, 2.0f * dt);
                player.velocity.y = Lerp(player.velocity.y, y, 2.0f * dt);
                player.velocity.z = Lerp(player.velocity.z, z, 2.0f * dt);
            }
            //


            //
            // // --- ground / surface clamping ---
            // // effectiveFloorHeight is set by GameData each frame:
            // //   - over a landing zone → max(terrain, deck) + wheel clearance
            // //   - elsewhere           → terrain height + wheel clearance
            // if (state.position.y < state.effectiveFloorHeight) {
            //     state.position.y = state.effectiveFloorHeight;
            //     // cancel any remaining downward velocity so the aircraft rests on the surface
            //     if (player.velocity.y < 0.0f) player.velocity.y = 0.0f;
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
