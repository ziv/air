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
        AircraftPhysicsSystem(registry, dt);
        VelocitySystem(registry, dt);
        PositionSystem(registry, dt);
    }

    void draw() const {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        RenderChroma(registry);
        EndDrawing();
    }
};
