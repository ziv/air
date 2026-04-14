module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module CameraSystem;

import WorldComponents;


export void CameraSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Orientation, PlayerView, View3D>();

    for (auto [entity, position, orientation, playerView, view3d]: view.each()) {
        // default first-person view: apply downward tilt around the right axis
        // the field of view is wide (above 70)

        playerView.camera.fovy = view3d.fov;
        playerView.camera.position = position.pos; //  + (Vector3){ 0.0f, 10.0f, 0.0f };

        playerView.camera.target = position.pos + orientation.forward;
        playerView.camera.up = orientation.up;

        // const Quaternion qTilt = QuaternionFromAxisAngle(orientation.right, -view3d.tilt);

        // playerView.camera.target = position.pos + Vector3RotateByQuaternion(orientation.forward, qTilt);
        // playerView.camera.up = Vector3RotateByQuaternion(orientation.up, qTilt);

        // playerView.camera.target = (Vector3){ 100.0f, 5.0f, 100.0f };
    }
}
