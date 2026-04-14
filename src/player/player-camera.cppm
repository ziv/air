module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PlayerCamera;

import WorldComponents;
import Helpers;

export class PlayerCamera {
    float tilt = 0.45f;
    float fov = 85.0f;
    Camera camera = {};

public:
    PlayerCamera() {
        camera.up = WorldUp();
        camera.fovy = fov;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    Camera &getCamera() { return camera; }

    void update(entt::registry &registry) {
        for (const auto view = registry.view<Player>(); auto [entity, player]: view.each()) {
            camera.position = player.pos;
            const Quaternion qTilt = QuaternionFromAxisAngle(player.right, -tilt);
            camera.target = camera.position + Vector3RotateByQuaternion(player.forward, qTilt);
            camera.up = Vector3RotateByQuaternion(player.up, qTilt);
        }
    }
};
