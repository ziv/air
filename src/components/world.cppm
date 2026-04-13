module;
#include <entt/entt.hpp>
#include <raymath.h>

export module WorldComponents;

// relationship between components

export struct Player {
};

export struct Parent {
    std::vector<entt::entity> children;
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

export struct Velocity {
    Vector3 velocity;
};

export struct AngularVelocity {
    Vector3 velocity;
};

export struct Acceleration {
    Vector3 linear;
};

export struct AngularAcceleration {
    Vector3 angular;
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
