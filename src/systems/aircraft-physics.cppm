module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include <algorithm>

export module AircraftPhysicsSystem;

import AircraftComponents;
import WorldComponents;

// CL = Cl0 + Cl_slope * alpha
float CalculateCL(const float aoaRadians, const float clZero, const float clSlope, const float stallAngle) {
    const float aoaDegrees = aoaRadians * RAD2DEG;

    if (std::abs(aoaDegrees) <= stallAngle) {
        return clZero + (clSlope * aoaDegrees);
    }

    const float liftAtStall = clZero + (clSlope * stallAngle);
    const float stallEffect = stallAngle / std::abs(aoaDegrees);

    return liftAtStall * stallEffect * 0.5f;
}

float CalculateCD(const float currentCl, const float cdZero, const float inducedDrag) {
    return cdZero + (inducedDrag * (currentCl * currentCl));
}

export void AircraftPhysicsSystem(entt::registry &registry, const float) {
    auto view = registry.view<Position3D, LinearVelocity, Aircraft, Engine, Orientation, LinerAcceleration, AngularAcceleration, AngularVelocity, AircraftControls, Grounded, AircraftUtils>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration, angularAcc, angularVel, controls, grounded, utils]: view.each()) {
        const auto mass = aircraft.weight / 9.81f; // weight in N
        const auto thrustForce = orientation.forward * (engine.thrust * engine.throttle);
        const auto weightForce = (Vector3){0.0f, -9.81f * mass, 0.0f};

        const auto speed = Vector3Length(velocity.velocity);

        // At near-zero speed aerodynamic forces are undefined (normalize → NaN).
        // Only thrust and gravity apply.
        if (speed < 0.001f) {
            acceleration.acc = (thrustForce + weightForce) / mass;
            angularAcc.acc = Vector3Zero();
            continue;
        }

        const auto squaredSpeed = speed * speed;
        const auto velocityDirection = Vector3Scale(velocity.velocity, 1.0f / speed);
        const auto liftDirection = Vector3Normalize(Vector3CrossProduct(Vector3CrossProduct(velocityDirection, orientation.up), velocityDirection));

        // Simplified angle-of-attack
        const float aoa = atan2f(Vector3DotProduct(velocity.velocity, orientation.up), Vector3DotProduct(velocity.velocity, orientation.forward));

        // Side-slip angle (beta) for weathervaning
        // Clamp dot product to [-1,1] to protect asinf from NaN on float imprecision
        const float sideSlipDot = std::clamp(Vector3DotProduct(velocityDirection, orientation.right), -1.0f, 1.0f);
        const float sideSlip = asinf(sideSlipDot);

        const float CL = CalculateCL(aoa, aircraft.cl, aircraft.liftSlopeCoefficient, aircraft.stallAngle);
        float CD = CalculateCD(CL, aircraft.cd, aircraft.inducedDragCoefficient);

        if (utils.brake) CD *= 3.0f;
        if (utils.gear) CD *= 1.5f;

        const auto dragForce = velocityDirection * -(CD * squaredSpeed);
        const auto liftForce = liftDirection * (CL * squaredSpeed);

        // Ground friction (rolling resistance) — constant force opposing motion
        const auto frictionForce = grounded.grounded
            ? velocityDirection * -(mass * 9.81f * (utils.brake ? 0.4f : 0.02f))
            : Vector3Zero();

        // --- Linear LinerAcceleration ---
        acceleration.acc = (thrustForce + dragForce + weightForce + liftForce + frictionForce) / mass;

        // --- Angular Mechanics (Torques & Angular LinerAcceleration) ---

        // Dynamic pressure (q = 0.5 * rho * v^2) dictates control authority
        // Using standard sea-level air density (1.225 kg/m^3)
        const float dynamicPressure = 0.5f * 1.225f * squaredSpeed;

        // 1. Aerodynamic Stability (Weathervaning & Pitch stability)
        // These torques naturally push the aircraft back to 0 AoA and 0 side-slip
        const float pitchStabilityTorque = -aoa * 2.0f /*aircraft.pitchStability*/;
        const float yawStabilityTorque = -sideSlip * 1.5f /*aircraft.yawStability*/;

        // 2. Control Torques (Player input)
        const float pitchInputTorque = controls.pitch * aircraft.pitchRatio;
        const float yawInputTorque = controls.yaw * aircraft.yawRatio;
        const float rollInputTorque = controls.roll * aircraft.rollRatio;

        // 3. Damping Torques (Air resistance slowing down rotation)
        // Opposes the current acc velocity
        const float pitchDamping = -angularVel.velocity.x * 3000.0f /*aircraft.pitchDamping*/;
        const float yawDamping = -angularVel.velocity.y * 3000.0f /*aircraft.yawDamping*/;
        const float rollDamping = -angularVel.velocity.z * 1000.0f /*aircraft.rollDamping*/;

        // 4. Calculate total torque per local axis (X = Pitch, Y = Yaw, Z = Roll)
        // Control and stability effectiveness scale with dynamic pressure, damping is absolute
        const Vector3 totalTorque = {
            ((pitchInputTorque + pitchStabilityTorque) * dynamicPressure) + pitchDamping,
            ((yawInputTorque + yawStabilityTorque) * dynamicPressure) + yawDamping,
            (rollInputTorque * dynamicPressure) + rollDamping
        };

        // 5. Calculate Angular LinerAcceleration (Torque / Moment of Inertia)
        // For a retro sim, deriving simple inertia from mass is sufficient.
        // Roll inertia is generally lower than pitch/yaw inertia in aircraft.
        const float pitchInertia = mass * 2.0f;
        const float yawInertia = mass * 2.0f;
        const float rollInertia = mass * 1.0f;

        angularAcc.acc.x = totalTorque.x / pitchInertia;
        angularAcc.acc.y = totalTorque.y / yawInertia;
        angularAcc.acc.z = totalTorque.z / rollInertia;

        if (grounded.grounded) {
            const float liftUp = Vector3DotProduct(liftForce, (Vector3){0.0f, 1.0f, 0.0f});
            if (liftUp > aircraft.weight) {
                grounded.grounded = false;
            } else {
                angularAcc.acc.z = 0.0f; // roll

                angularVel.velocity.z = 0.0f;
            }
        } else if (position.pos.y <= 0.0f) {
            grounded.grounded = true;
        }
    }
}