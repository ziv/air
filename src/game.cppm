module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import UserInputs;
import WorldComponents;
import PlayerPrefabs;
import ScenePrefabs;
import RenderCockpit;
import RenderModel;
import RenderDebug;
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

public:
    explicit Game(const JsonConfig &cfg, entt::registry &reg) : registry(reg), config(cfg) {
        Factories::createPlayer(registry, config);
        Factories::createScene(registry, config);
    }

    void update() const {
        const float dt = GetFrameTime();

        UserInputs(registry, dt);
        AircraftMechanics(registry, dt);
        // AircraftPhysicsSystem(registry, dt);
        AircraftSimplePhysicsSystem(registry, dt);
        VelocitySystem(registry, dt);
        RotationSystem(registry, dt);
        PositionSystem(registry, dt);
        CameraSystem(registry, dt);
    }

    void draw() const {
        const auto view = registry.view<PlayerView>();
        auto &[camera] = view.get<PlayerView>(view.front());

        BeginDrawing();
        ClearBackground(BLUE);
        BeginMode3D(camera);
        DrawGrid(100, 1);
        // RenderModel(registry);
        EndMode3D();
        // RenderCockpit(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
        EndDrawing();
    }
};
