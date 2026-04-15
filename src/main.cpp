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
        auto [title, width, height,nearPlane, farPlane] = config.get<WindowConfig>("/window");

        InitAudioDevice();
        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);

        TraceLog(LOG_DEBUG, "Setting near plane to %f and far plane to %f", nearPlane, farPlane);
        rlSetClipPlanes(nearPlane, farPlane);

        // set registry & resource loader
        entt::registry registry;
        registry.ctx().emplace<ResourceManager>();

        // load scenario data
        const JsonConfig scenario("assets/scenario.jsonc");

        Game game(config, registry, scenario.get<Scenario>("/data"));

        while (!WindowShouldClose()) {
            game.update();
            BeginDrawing();
            game.draw();
            EndDrawing();
        }
        CloseWindow();
        CloseAudioDevice();
    } catch (std::exception &e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
