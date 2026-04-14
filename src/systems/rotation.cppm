module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RotationSystem;

import Constants;
import AircraftComponents;
import WorldComponents;


export void RotationSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Orientation, Rotation, AngularVelocity, AngularAcceleration>();

    for (auto [entity, orientation, rotation, angVel, angAcc]: view.each()) {
        // 1. Update Angular Velocity based on Angular Acceleration
        // Integration: Velocity = Velocity + (Acceleration * dt)
        angVel.velocity = Vector3Add(angVel.velocity, Vector3Scale(angAcc.angular, dt));

        // 2. Create a rotation delta for the current frame
        // raymath's QuaternionFromEuler expects radians: (pitch, yaw, roll)
        // which corresponds to our (x, y, z)
        // const Quaternion frameRotation = QuaternionFromEuler(
        //     angVel.velocity.x * dt,
        //     angVel.velocity.y * dt,
        //     angVel.velocity.z * dt
        // );

        // 2. New one
        const auto qPitch = QuaternionFromAxisAngle(orientation.right, angVel.velocity.x * dt);
        const auto qYaw = QuaternionFromAxisAngle(orientation.up, angVel.velocity.y * dt);
        const auto qRoll = QuaternionFromAxisAngle(orientation.forward, angVel.velocity.z * dt);

        const auto frameRotation = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));


        // 3. Apply the frame rotation to our global orientation
        // Note: Order matters in Quaternion multiplication
        rotation.rotation = QuaternionMultiply(rotation.rotation, frameRotation);

        // 4. Normalize the quaternion to prevent numerical drift over time
        // This ensures the aircraft model doesn't "shrink" or "deform"
        rotation.rotation = QuaternionNormalize(rotation.rotation);

        // 5. Update helper direction vectors from the new quaternion
        // These are essential for the PhysicsSystem's next frame (lift/drag directions)
        // Assuming: Forward is +Z, Up is +Y, Right is +X in your coordinate system
        orientation.forward = Vector3RotateByQuaternion(Constants::WorldForward, rotation.rotation);
        orientation.up = Vector3RotateByQuaternion(Constants::WorldUp, rotation.rotation);
        orientation.right = Vector3RotateByQuaternion(Constants::WorldRight, rotation.rotation);
    }
}
