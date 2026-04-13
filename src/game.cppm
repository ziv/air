module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import PlayerPrefabs;
import ScenePrefabs;
import ChromaRender;
import AircraftPhysicsSystem;
import PositionSystem;
import VelocitySystem;
import UserInputs;
import RotationSystem;
import AircraftMechanics;

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
        AircraftPhysicsSystem(registry, dt);
        VelocitySystem(registry, dt);
        RotationSystem(registry, dt);
        PositionSystem(registry, dt);
    }

    void draw() const {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        RenderCockpit(registry);
        EndDrawing();
    }
};
