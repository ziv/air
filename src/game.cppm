module;
#include <entt/entt.hpp>
#include <utility>
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
import Views;


export class Game {
    entt::registry &registry;
    Scenario scenario;

    PlayerControls playerControls;
    PlayerPhysics playerPhysics;
    PlayerPosition playerPosition;
    PlayerRotation playerRotation;
    PlayerCamera playerCamera;
    PlayerGroundCheck playerGroundCheck;

    std::vector<std::unique_ptr<ViewBase> > views;

public:
    explicit Game(const JsonConfig &cfg,
                  const JsonConfig &scn,
                  entt::registry &reg)
        : registry(reg),
          scenario(scn.get<Scenario>("/data")),
          playerControls(cfg.get<PlayerControlsConfig>("/player/controls")),
          playerPhysics(cfg.get<PlayerPhysicsConfig>("/player/aircraft")),
          playerPosition(cfg.get<PlayerPositionConfig>("/player/position")),
          playerRotation(cfg.get<PlayerTransformationConfig>("/player/aircraft")),
          playerCamera(cfg.get<PlayerCameraConfig>("/player/camera")),
          playerGroundCheck(cfg.get<PlayerGroundCheckConfig>("/player/groundCheck")) {
        // set initial offset
        registry.ctx().emplace<Offset>(Vector3Zero());

        views.push_back(std::make_unique<HudView>(cfg.get<GlobalConfig>("/global"), cfg.get<HudConfig>("/views/hud")));

        Factories::createPlayer(registry, cfg, scenario);
        Factories::createScene(registry, cfg);
        Factories::createCockpit(registry, cfg);

        // spawn all items from scenario
        for (const auto &def: scenario.entities) {
            Factories::createUnit(registry, def);
        }
    }

    void update() {
        const float dt = GetFrameTime();

        const auto id = registry.ctx().get<PlayerEntity>().id;
        if (registry.all_of<Crashed>(id)) return;

        playerControls.update(registry, dt);
        playerPhysics.update(registry, dt);
        playerPosition.update(registry, dt);
        playerRotation.update(registry, dt);
        playerCamera.update(registry, dt);
        playerGroundCheck.update(registry, dt);
    }

    void draw() {
        ClearBackground(scenario.skyColor);
        // ClearBackground(BLUE);

        // 3D
        BeginMode3D(playerCamera.getCamera());
        WorldStreamerSystem(registry);
        RenderModels(registry);
        RenderDebugging(registry);
        EndMode3D();

        // 2D
        RenderCockpit(registry);
        for (const auto &v: views) v->draw(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
