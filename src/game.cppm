module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import PlayerControls;
import PlayerCamera;
import PlayerPhysics;
import PlayerRotation;
import UserInputs;
import WorldComponents;
import PlayerPrefabs;
import ScenePrefabs;
import RenderCockpit;
import RenderModel;
import RenderDebug;
import RenderHudLadder;
import WorldStreamerSystem;
import AircraftPhysicsSystem;
import AircraftSimplePhysicsSystem;
import PositionSystem;
import VelocitySystem;
import RotationSystem;
import CameraSystem;
import AircraftMechanicsSystem;

export class Game {
    entt::registry &registry;
    const JsonConfig &config;

    PlayerControls playerControls;
    PlayerPhysics playerPhysics;
    PlayerRotation playerRotation;
    PlayerCamera playerCamera;

public:
    explicit Game(const JsonConfig &cfg, entt::registry &reg) : registry(reg), config(cfg) {
        Factories::createPlayer(registry, config);
        // Factories::createScene(registry, config);
    }

    void update() {
        const float dt = GetFrameTime();

        playerControls.update(registry, dt);
        playerPhysics.update(registry, dt);
        playerRotation.update(registry, dt);
        playerCamera.update(registry);
        // UserInputs(registry, dt);
        // AircraftMechanics(registry, dt);
        // // AircraftPhysicsSystem(registry, dt);
        // AircraftSimplePhysicsSystem(registry, dt);
        // VelocitySystem(registry, dt);
        // RotationSystem(registry, dt);
        // PositionSystem(registry, dt);
        // CameraSystem(registry, dt);
    }

    void draw() {
        // const auto view = registry.view<PlayerView>();
        // auto &[camera] = view.get<PlayerView>(view.front());

        ClearBackground(BLUE);
        BeginMode3D(playerCamera.getCamera());
        DrawGrid(1000, 10.0f);
        // WorldStreamerSystem(registry);
        // RenderModel(registry);

        // debug cubes for reference
        for (int x = 0; x < 10; ++x) {
            for (int z = 0; z < 10; ++z) {
                DrawCube(Vector3{static_cast<float>(x * 100), 0.0f, static_cast<float>(z * 100)},
                         3.0f, 3.0f, 3.0f, x % 2 ? BLUE : RED);
            }
        }

        EndMode3D();
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
