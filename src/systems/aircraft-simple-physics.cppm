module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include <algorithm>

export module AircraftSimplePhysicsSystem;

import AircraftComponents;
import WorldComponents;

export void AircraftSimplePhysicsSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity, Aircraft, Engine, Orientation, Acceleration, AngularAcceleration, AngularVelocity, AircraftControls, Grounded, AircraftUtils>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration, angularAcc, angularVel, controls, grounded, utils]: view.each()) {
        const auto mass = aircraft.weight / 9.81f;
        const auto thrustForce = orientation.forward * (engine.thrust * engine.throttle);
        const auto weightForce = (Vector3){0.0f, -9.81f * mass, 0.0f};

        const auto speed = Vector3Length(velocity.velocity);

        if (speed < 0.001f) {
            acceleration.linear = (thrustForce + weightForce) / mass;
            angularAcc.angular = Vector3Zero();
            continue;
        }

        const auto velocityDirection = Vector3Scale(velocity.velocity, 1.0f / speed);

        // Simple drag: constant coefficient opposing velocity
        const auto dragForce = velocityDirection * -(aircraft.cd * speed * speed);

        // Simple lift: proportional to speed squared, always in orientation.up
        const float liftMag = aircraft.cl * speed * speed;
        const auto liftForce = orientation.up * liftMag;

        // Ground friction
        const auto frictionForce = grounded.grounded
            ? velocityDirection * -(mass * 9.81f * (utils.brake ? 0.4f : 0.02f))
            : Vector3Zero();

        // --- Linear Acceleration ---
        acceleration.linear = (thrustForce + dragForce + weightForce + liftForce + frictionForce) / mass;

        // --- Angular Acceleration (input + damping)
        angularAcc.angular = {
            controls.pitch * aircraft.pitchRatio - angularVel.velocity.x * 5.0f,
            controls.yaw * aircraft.yawRatio - angularVel.velocity.y * 5.0f,
            controls.roll * aircraft.rollRatio - angularVel.velocity.z * 3.0f
        };

        // Ground constraints
        if (grounded.grounded) {
            // Lift off when upward lift component exceeds gravity
            const float liftUp = Vector3DotProduct(liftForce, (Vector3){0.0f, 1.0f, 0.0f});
            if (liftUp > aircraft.weight) {
                grounded.grounded = false;
            } else {
                angularAcc.angular.z = 0.0f;
                angularVel.velocity.z = 0.0f;
            }
        } else if (position.pos.y <= 0.0f) {
            grounded.grounded = true;
        }
    }
}