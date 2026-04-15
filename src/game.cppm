module;
#include <entt/entt.hpp>
#include "lib/ray.hpp"

export module Game;

import JsonConfig;
import Player;
import Components;
import Prefabs;
import WorldStreamerSystem;
import RenderSystem;
import RenderCockpit;
import Types;

export class Game {
    entt::registry &registry;
    const JsonConfig &config;
    const Scenario scenario;

    PlayerControls playerControls;
    PlayerPhysics playerPhysics;
    PlayerPosition playerPosition;
    PlayerRotation playerRotation;
    PlayerCamera playerCamera;

    float stallSpeed;

public:
    explicit Game(const JsonConfig &cfg,
                  entt::registry &reg,
                  const Scenario &sc)
        : registry(reg),
          config(cfg),
          scenario(sc),
          playerControls(cfg.get<PlayerControlsConfig>("/player/controls")),
          playerPhysics(cfg.get<PlayerPhysicsConfig>("/player/aircraft")),
          playerPosition(cfg.get<PlayerPositionConfig>("/player/position")),
          playerRotation(cfg.get<PlayerTransformationConfig>("/player/aircraft")),
          playerCamera(cfg.get<PlayerCameraConfig>("/player/camera")),
          stallSpeed(cfg.get<float>("/player/aircraft/stallSpeed")) {
        // set initial offset
        registry.ctx().emplace<Offset>(Vector3Zero());

        Factories::createPlayer(registry, config, scenario);
        Factories::createScene(registry, config);
        Factories::createCockpit(registry, config);

        // spawn all items from scenario
        for (const auto &def: scenario.entities) {
            Factories::createUnit(registry, def);
        }
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
        ClearBackground(BLUE);
        BeginMode3D(playerCamera.getCamera());
        WorldStreamerSystem(registry);
        RenderModels(registry);
        RenderDebugging(registry);
        EndMode3D();
        RenderCockpit(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
