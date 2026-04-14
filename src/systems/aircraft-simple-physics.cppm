module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include <algorithm>

export module AircraftSimplePhysicsSystem;

import AircraftComponents;
import WorldComponents;
// import Constants;

export void AircraftSimplePhysicsSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, LinearVelocity, Aircraft, Engine, Orientation, LinerAcceleration, AngularAcceleration, AngularVelocity,
        AircraftControls,
        Grounded, AircraftUtils>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration, angularAcc, angularVel, controls, grounded, utils]: view.each()) {
        constexpr Vector3 Gravity = {0.0f, -9.81f, 0.0f};
        const auto speed = velocity.speed;
        const auto squaredSpeed = speed * speed;

        const auto thrust = engine.throttle * engine.thrust;
        const auto drag = squaredSpeed * aircraft.cd;
        const auto lift = squaredSpeed * aircraft.cl;

        // todo handle brakes and gear open

        const auto mass = aircraft.weight /*Newton*/ / 9.81f;

        const auto thrustForce = orientation.forward * thrust;
        const auto dragForce = orientation.forward * -drag;
        const auto liftForce = orientation.up * lift;
        const auto weightForce = Gravity * mass;

        if (speed < 0.01f) {
            acceleration.acc = (thrustForce + weightForce) / mass;
            angularVel.velocity = Vector3Zero();
            continue;
        }

        const auto total = thrustForce + dragForce + weightForce + liftForce;
        acceleration.acc = total / mass;

        // instead of calculating angular acceleration we use the user inputs
        angularVel.velocity.z = controls.roll * aircraft.rollRatio;
        angularVel.velocity.x = controls.pitch * aircraft.pitchRatio;
        angularVel.velocity.y = controls.yaw * aircraft.yawRatio;


        // // aircraft.weight and thrust are in Newtons
        // const auto mass = aircraft.weight / 9.81f;
        // const auto thrustForce = orientation.forward * (engine.thrust * engine.throttle);
        // const auto weightForce = (Vector3){0.0f, -9.81f * mass, 0.0f};
        //
        // const auto speed = Vector3Length(velocity.velocity);
        //
        // if (speed < 0.001f) {
        //     acceleration.acc = (thrustForce + weightForce) / mass;
        //     angularAcc.acc = Vector3Zero();
        //     continue;
        // }
        //
        // const auto velocityDirection = Vector3Scale(velocity.velocity, 1.0f / speed);
        //
        // // simple drag: constant coefficient opposing velocity
        // const auto dragForce = velocityDirection * -(aircraft.cd * speed * speed);
        //
        // // simple lift: proportional to speed squared, scaled by how much the wings face world-up
        // // (avoids lift pushing the aircraft upward when pitched nose-down)
        // const auto liftAlignment = Vector3DotProduct(orientation.up, {0.0f, 1.0f, 0.0f});
        // const auto liftForce = (Vector3){0.0f, aircraft.cl * speed * speed * liftAlignment, 0.0f};
        //

        //
        // // linear acceleration
        // acceleration.acc = (thrustForce + dragForce + weightForce + liftForce + frictionForce) / mass;
        //
        // // instead of using acc acceleration, we set the input into acc velocity
        // constexpr float maxRollRate = 3.5f; // rad/s (~200 deg/s)
        // constexpr float maxPitchRate = 2.5f; // rad/s (~143 deg/s)
        // constexpr float maxYawRate = 1.2f; // rad/s (~69 deg/s)
        //
        // const auto targetRollVelocity = controls.roll * maxRollRate;
        // const auto targetPitchVelocity = controls.pitch * maxPitchRate;
        // const auto targetYawVelocity = controls.yaw * maxYawRate;
        //
        // // framerate-independent lerp: alpha = 1 - (1-ratio)^dt
        // const auto rollAlpha = 1.0f - std::pow(1.0f - aircraft.rollRatio, dt);
        // const auto pitchAlpha = 1.0f - std::pow(1.0f - aircraft.pitchRatio, dt);
        // const auto yawAlpha = 1.0f - std::pow(1.0f - aircraft.yawRatio, dt);
        //
        // angularVel.velocity.z = std::lerp(angularVel.velocity.z, targetRollVelocity, rollAlpha);
        // angularVel.velocity.x = std::lerp(angularVel.velocity.x, targetPitchVelocity, pitchAlpha);
        // angularVel.velocity.y = std::lerp(angularVel.velocity.y, targetYawVelocity, yawAlpha);
        //
        //
        // // on ground there is no more acceleration down
        // if (grounded.grounded && acceleration.acc.y < 0.0f) acceleration.acc.y = 0.0f;
    }
}
