#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>
#include <string>
#include "lib/ray-logger.hpp"
#include "rlgl.h"

import JsonConfig;
import ConfigTypes;
import Game;
import ResourceManager;

int main() {
    SetTraceLogCallback(CustomLogCallback);
    try {
        const JsonConfig config("assets/config.jsonc");
        auto [title, width, height,nearPlane, farPlane] = config.get<WindowConfig>("/window");

        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);

        rlSetClipPlanes(nearPlane, farPlane);

        entt::registry registry;

        // keep the resource manager in the registry so it
        // can be easily accessed from anywhere and automatically
        // destroyed at the end of the program
        // example:
        // ```
        // auto& assets = registry.ctx().get<ResourceManager>();
        //
        // auto modelId = entt::hashed_string("player_plane");
        // assets.models.load(modelId, ....);
        // registry.emplace<Modeled>(player, assets.models.handle(modelId));
        // ```
        registry.ctx().emplace<ResourceManager>();

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
