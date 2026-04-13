module;
#include <entt/entt.hpp>
#include <raylib.h>

export module Game;

import JsonConfig;
import PlayerPrefabs;
import ScenePrefabs;
import ChromaRender;

export class Game {
    entt::registry &registry;
    const JsonConfig &config;

public:
    explicit Game(const JsonConfig &cfg, entt::registry &reg) : registry(reg), config(cfg) {
        Factories::createPlayer(registry, config);
        Factories::createScene(registry, config);
    }

    void draw() {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        RenderChroma(registry);
        EndDrawing();
    }
};
