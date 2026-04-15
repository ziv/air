module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Components:World;

// specific player/world structures

export struct World {
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
};

// global user offset

export struct Offset {
    Vector3 offset;
};

export struct Identify {
    std::string name;
};

// position

export struct Position3D {
    Vector3 pos;
    Vector3 offset;
};

export struct Position2D {
    Vector2 pos;
};

// dimensions

export struct Dimensions3D {
    Vector3 size;
};

export struct Dimensions2D {
    Vector2 size;
};

// movement

export struct LinearVelocity {
    Vector3 velocity;
    float speed;
};

export struct AngularVelocity {
    Vector3 velocity;
};

export struct LinerAcceleration {
    Vector3 acc;
};

export struct AngularAcceleration {
    Vector3 acc;
};

export struct Rotation {
    Quaternion rotation;
};

export struct Orientation {
    Vector3 forward;
    Vector3 up;
    Vector3 right;
};
