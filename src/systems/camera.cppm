module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module CameraSystem;

import WorldComponents;


export void CameraSystem(entt::registry &registry, float dt) {
    auto view = registry.view<Position3D, Orientation, PlayerView, View3D>();

    for (auto [entity, position, orientation, playerView, view]: view.each()) {
        playerView.camera.fovy = view.fov;
        playerView.camera.position = position.pos + (Vector3){0.0f, 3.0f, 0.0f};

        const Quaternion qTilt = QuaternionFromAxisAngle(orientation.right, -view.tilt);

        playerView.camera.target = position.pos + Vector3RotateByQuaternion(orientation.forward, qTilt);
        playerView.camera.up = Vector3RotateByQuaternion(orientation.up, qTilt);
    }
}
