module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module WorldComponents;

// relationship between components

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

export struct PlayerView {
    Camera3D camera;
};

export struct CameraComponent {
    entt::entity handle{entt::null};
};

export struct ParentOf {
    entt::entity parent{entt::null};
};

export struct ChildOf {
    entt::entity parent{entt::null};
};

export struct World {
};

export struct Tile {
    int x;
    int z;
};

export struct ActiveTileTag {
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

export struct View3D {
    float fov;
    float tilt;
};
