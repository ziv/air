module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import Player;

import Prefabs;

import WorldStreamerSystem;

import RenderSystem;
import RenderCockpit;
import Types;

export class Game {
    entt::registry &registry;
    const JsonConfig &config;
    const Scenario scenario;

    PlayerControls playerControls{};
    PlayerPhysics playerPhysics;
    PlayerPosition playerPosition;
    PlayerRotation playerRotation;
    PlayerCamera playerCamera;

public:
    explicit Game(const JsonConfig &cfg,
                  entt::registry &reg,
                  const Scenario &sc)
        : registry(reg),
          config(cfg),
          scenario(sc),
          playerPhysics(cfg.get<PlayerPhysicsConfig>("/player/aircraft")),
          playerRotation(cfg.get<PlayerTransformationConfig>("/player/aircraft")),
          playerCamera(cfg.get<PlayerCameraConfig>("/player/camera")) {
        Factories::createPlayer(registry, config);
        Factories::createScene(registry, config);
        Factories::createCockpit(registry, config);

        // spawn all items from scenario
        for (const auto &def: scenario.entities) {
            Factories::createUnit(registry, def);
            TraceLog(LOG_DEBUG, "Entity loaded with %s", def.id.c_str());
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
        // const auto view = registry.view<PlayerView>();
        // auto &[camera] = view.get<PlayerView>(view.front());

        ClearBackground(BLUE);
        BeginMode3D(playerCamera.getCamera());
            WorldStreamerSystem(registry);
            RenderModel(registry);
            RenderDebugging(registry);
        EndMode3D();
        RenderCockpit(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }

    void spawn(const EntityDef &def) {
        TraceLog(LOG_DEBUG, "Spawning entity: %s", def.id.c_str());
    }
};
