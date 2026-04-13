#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>
#include <string>
#include "lib/ray-logger.hpp"

import JsonConfig;
import ConfigTypes;
import Game;

int main() {
    SetTraceLogCallback(CustomLogCallback);
    try {
        const JsonConfig config("assets/config.jsonc");
        auto [title, width, height] = config.get<WindowConfig>("/window");

        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);

        entt::registry registry;
        Game game(config, registry);

        while (!WindowShouldClose()) {
            game.update();
            game.draw();
        }
        CloseWindow();
    } catch (std::exception &e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
