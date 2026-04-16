module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

/// Only data & tags components
export module Components:Data;

export struct PlayerEntity {
    entt::entity id;
};

export struct Player {
    Quaternion rotation;
    Vector3 pos;
    Vector3 offset;
    Vector3 velocity;
    Vector3 forward;
    Vector3 up;
    Vector3 right;
    float speed;
};

export struct PlayerInputs {
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float throttle = 0.0f;
    bool gear = true;
    bool brakes = true;
    bool autopilot = false;
};

export struct GroundHeight {
    float height;
    float effectiveGroundHeight;
};

export struct Offset {
    Vector3 offset;
};
