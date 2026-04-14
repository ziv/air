module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include <algorithm>

export module AircraftSimplePhysicsSystem;

import AircraftComponents;
import WorldComponents;

export void AircraftSimplePhysicsSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, LinearVelocity, Aircraft, Engine, Orientation, LinerAcceleration, AngularAcceleration, AngularVelocity,
        AircraftControls,
        Grounded, AircraftUtils>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration, angularAcc, angularVel, controls, grounded, utils]: view.each()) {
        // aircraft.weight and thrust are in Newtons
        const auto mass = aircraft.weight / 9.81f;
        const auto thrustForce = orientation.forward * (engine.thrust * engine.throttle);
        const auto weightForce = (Vector3){0.0f, -9.81f * mass, 0.0f};

        const auto speed = Vector3Length(velocity.velocity);

        if (speed < 0.001f) {
            acceleration.acc = (thrustForce + weightForce) / mass;
            angularAcc.acc = Vector3Zero();
            continue;
        }

        const auto velocityDirection = Vector3Scale(velocity.velocity, 1.0f / speed);

        // simple drag: constant coefficient opposing velocity
        const auto dragForce = velocityDirection * -(aircraft.cd * speed * speed);

        // simple lift: proportional to speed squared, always in orientation.up
        const auto liftForce = orientation.up * (aircraft.cl * speed * speed);

        // ground friction
        const auto frictionForce = grounded.grounded
                                       ? velocityDirection * -(mass * 9.81f * (utils.brake ? 0.4f : 0.02f))
                                       : Vector3Zero();

        // linear acceleration
        acceleration.acc = (thrustForce + dragForce + weightForce + liftForce + frictionForce) / mass;

        // instead of using acc acceleration, we set the input into acc velocity
        constexpr float maxRollRate = 1.0f;
        constexpr float maxPitchRate = 1.0f;
        constexpr float maxYawRate = 0.5f;

        const auto targetRollVelocity = controls.roll * maxRollRate;
        const auto targetPitchVelocity = controls.pitch * maxPitchRate;
        const auto targetYawVelocity = controls.yaw * maxYawRate;

        angularVel.velocity.z = std::lerp(angularVel.velocity.z, targetRollVelocity, aircraft.rollRatio);
        angularVel.velocity.x = std::lerp(angularVel.velocity.x, targetPitchVelocity, aircraft.pitchRatio);
        angularVel.velocity.y = std::lerp(angularVel.velocity.y, targetYawVelocity, aircraft.yawRatio);


        // on ground there is no more acceleration down
        if (grounded.grounded && acceleration.acc.y < 0.0f) acceleration.acc.y = 0.0f;
    }
}
