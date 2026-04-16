#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>
#include <string>
#include "lib/ray-logger.hpp"
#include "rlgl.h"

import JsonConfig;
import Types;
import Game;
import ResourceManager;

int main() {
    SetTraceLogCallback(CustomLogCallback);
    SetTraceLogLevel(LOG_DEBUG);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    try {
        const JsonConfig config("assets/config.jsonc");
        const auto conf = config.get<GlobalConfig>("/global");

        InitAudioDevice();
        InitWindow(conf.width, conf.height, conf.title.c_str());
        SetTargetFPS(60);

        TraceLog(LOG_DEBUG, "Setting near plane to %f and far plane to %f", conf.nearPlane, conf.farPlane);
        rlSetClipPlanes(conf.nearPlane, conf.farPlane);

        // set registry & resource loader & global config
        entt::registry registry;
        registry.ctx().emplace<ResourceManager>();
        registry.ctx().emplace<GlobalConfig>(conf);

        // load scenario data
        const JsonConfig scenarioDef("assets/scenario.jsonc");

        Game game(config, scenarioDef, registry);

        while (!WindowShouldClose()) {
            game.update();
            BeginDrawing();
            game.draw();
            EndDrawing();
        }

        registry.ctx().erase<ResourceManager>();
        registry.clear();

        CloseWindow();
        CloseAudioDevice();
    } catch (std::exception &e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
