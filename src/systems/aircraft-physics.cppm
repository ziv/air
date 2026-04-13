module;
#include <entt/entt.hpp>
#include <raymath.h>

export module AircraftPhysicsSystem;

import Constants;
import AircraftComponents;
import WorldComponents;

export void AircraftPhysicsSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity, Aircraft, Engine, Orientation, Acceleration>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration]: view.each()) {
        const auto speed = Vector3Length(velocity.velocity);
        const auto squaredSpeed = speed * speed;
        const auto moveDirection = Vector3Normalize(velocity.velocity); // required for drag

        const auto thrust = engine.thrust * engine.throttle;
        const auto drag = aircraft.cd * squaredSpeed;
        const auto lift = aircraft.cl * squaredSpeed;
        const auto mass = aircraft.weight / 9.81f;

        const auto thrustForce = orientation.forward * thrust;
        const auto dragForce = moveDirection * -drag;
        const auto liftForce = orientation.up * lift;
        const auto weightForce = (Vector3){0.0f, -9.81f, 0.0f} * mass;

        const auto total = thrustForce + dragForce + weightForce + liftForce;

        acceleration.acceleration = total * 1 / mass;

        // todo maybe should be done in 2 different systems (1 for forces, 1 for movement and position (ground effects)
        // velocity.velocity = velocity.velocity + (acceleration * dt);
        // position.pos = position.pos + (velocity.velocity * dt);

        // this is an update mechanism for the listeners
        // registry.patch<Velocity>(entity, [acceleration, dt](auto &vel) {
        //     vel.velocity = vel.velocity + (acceleration * dt);
        // });
        //
        //
        // registry.patch<Position3D>(entity, [dt](auto &pos, const auto &vel) {
        //     pos.position = pos.position + (vel.velocity * dt);
        // });
    }
}
