module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import Player;

import PlayerPrefabs;
import ScenePrefabs;
import CockpitPrefabs;

import WorldStreamerSystem;

import RenderDebug;
import RenderCockpit;

export class Game {
    entt::registry &registry;
    const JsonConfig &config;

    PlayerControls playerControls;
    PlayerPhysics playerPhysics;
    PlayerPosition playerPosition;
    PlayerRotation playerRotation;
    PlayerCamera playerCamera;

public:
    explicit Game(const JsonConfig &cfg, entt::registry &reg)
        : registry(reg),
          config(cfg),
          playerCamera(cfg.get<PlayerCameraConfig>("/player/camera")),
          playerPhysics(cfg.get<PlayerPhysicsConfig>("/player/aircraft")),
          playerRotation(cfg.get<PlayerTransformationConfig>("/player/aircraft")) {
        Factories::createPlayer(registry, config);
        Factories::createScene(registry, config);
        Factories::createCockpit(registry, config);
    }

    void update() {
        const float dt = GetFrameTime();

        playerControls.update(registry, dt);
        playerPhysics.update(registry, dt);
        playerPosition.update(registry, dt);
        playerRotation.update(registry, dt);
        playerCamera.update(registry);
    }

    void draw() {
        // const auto view = registry.view<PlayerView>();
        // auto &[camera] = view.get<PlayerView>(view.front());

        ClearBackground(BLUE);
        BeginMode3D(playerCamera.getCamera());
        DrawGrid(100, 100.0f);
        WorldStreamerSystem(registry);
        // RenderModel(registry);

        // debug cubes for reference
        // for (int x = 0; x < 10; ++x) {
        //     for (int z = 0; z < 10; ++z) {
        //         DrawCube(Vector3{static_cast<float>(x * 100), 0.0f, static_cast<float>(z * 100)},
        //                  3.0f, 3.0f, 3.0f, x % 2 ? BLUE : RED);
        //     }
        // }

        EndMode3D();
        RenderCockpit(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
