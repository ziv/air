module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RotationSystem;

import Constants;
import AircraftComponents;
import WorldComponents;


export void RotationSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Orientation, Rotation, AngularVelocity>();

    for (auto [entity, orientation, rotation, angVel]: view.each()) {
        // Take the "acc velocity" and rotate
        const auto qPitch = QuaternionFromAxisAngle(orientation.right, angVel.velocity.x * dt);
        const auto qYaw = QuaternionFromAxisAngle(orientation.up, angVel.velocity.y * dt);
        const auto qRoll = QuaternionFromAxisAngle(orientation.forward, angVel.velocity.z * dt);

        const auto frameRotation = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

        // apply the frame rotation to our global orientation
        // order matters in Quaternion multiplication (notice the order of arguments)
        rotation.rotation = QuaternionMultiply(frameRotation, rotation.rotation);

        // normalize the quaternion to prevent numerical drift over time
        rotation.rotation = QuaternionNormalize(rotation.rotation);

        // update helper direction vectors from the new quaternion
        // these are essential for the PhysicsSystem's next frame (lift/drag directions)
        // Forward is +Z, Up is +Y, Right is +X in our coordinate system
        orientation.forward = Vector3RotateByQuaternion(Constants::WorldForward, rotation.rotation);
        orientation.up = Vector3RotateByQuaternion(Constants::WorldUp, rotation.rotation);
        orientation.right = Vector3RotateByQuaternion(Constants::WorldRight, rotation.rotation);
    }
}
