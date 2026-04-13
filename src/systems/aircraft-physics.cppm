module;
#include <entt/entt.hpp>
#include <raymath.h>

export module AircraftPhysicsSystem;

import Constants;
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

export void AircraftPhysicsSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Velocity, Aircraft, Engine, Orientation, Acceleration, AngularAcceleration, AngularVelocity, AircraftControls>();

    for (auto [entity, position, velocity, aircraft, engine, orientation, acceleration, angularAcc, angularVel, controls]: view.each()) {
        const auto speed = Vector3Length(velocity.velocity);

        if (speed < 0.1f) continue;

        const auto squaredSpeed = speed * speed;

        const auto velocityDirection = Vector3Normalize(velocity.velocity);
        const auto liftDirection = Vector3Normalize(Vector3CrossProduct(Vector3CrossProduct(velocityDirection, orientation.up), velocityDirection));

        // Simplified angle-of-attack
        const float aoa = atan2f(Vector3DotProduct(velocity.velocity, orientation.up), Vector3DotProduct(velocity.velocity, orientation.forward));

        // Side-slip angle (beta) for weathervaning
        const float sideSlip = asinf(Vector3DotProduct(velocityDirection, orientation.right));

        const float CL = CalculateCL(aoa, aircraft.cl, aircraft.liftSlopeCoefficient, aircraft.stallAngle);
        const float CD = CalculateCD(CL, aircraft.cd, aircraft.inducedDragCoefficient);

        const auto thrust = engine.thrust * engine.throttle;
        const auto drag = CD * squaredSpeed;
        const auto lift = CL * squaredSpeed;
        const auto mass = aircraft.weight / 9.81f;

        const auto thrustForce = orientation.forward * thrust;
        const auto dragForce = velocityDirection * -drag;
        const auto liftForce = liftDirection * lift;
        const auto weightForce = (Vector3){0.0f, -9.81f * mass, 0.0f};

        // --- Linear Acceleration ---
        acceleration.linear = (thrustForce + dragForce + weightForce + liftForce) / mass;


        // --- Angular Mechanics (Torques & Angular Acceleration) ---

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
        // Opposes the current angular velocity
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

        // 5. Calculate Angular Acceleration (Torque / Moment of Inertia)
        // For a retro sim, deriving simple inertia from mass is sufficient.
        // Roll inertia is generally lower than pitch/yaw inertia in aircraft.
        const float pitchInertia = mass * 2.0f;
        const float yawInertia = mass * 2.0f;
        const float rollInertia = mass * 1.0f;

        angularAcc.angular.x = totalTorque.x / pitchInertia;
        angularAcc.angular.y = totalTorque.y / yawInertia;
        angularAcc.angular.z = totalTorque.z / rollInertia;
    }
}
