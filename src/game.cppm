module;
#include <entt/entt.hpp>
#include "lib/ray.hpp"

export module Game;

import JsonConfig;
import Player;
import Components;
import Prefabs;
import WorldStreamerSystem;
import RenderSystem;
import Types;
import ResourceManager;
import WidgetsInputs;


export class Game {
    entt::registry &registry;
    Scenario scenario{};
    PlayerDispatcher dispatcher;

public:
    explicit Game(const JsonConfig &cfg,
                  const JsonConfig &scn,
                  entt::registry &reg)
        : registry(reg),
          scenario(scn.get<Scenario>("/data")),
          dispatcher(cfg) {
        // set initial offset
        registry.ctx().emplace<Offset>(Vector3Zero());

        Factories::createPlayer(registry, cfg, scenario);
        Factories::createScene(registry, cfg);
        Factories::createCockpit(registry, cfg);
        Factories::create_hud(registry, cfg);
        Factories::createCockpitWidgets(registry);

        Updates::setMinimap(0, registry, cfg);
        Updates::setEngineStatus(1, registry);
        Updates::setRadar(2, registry, cfg);

        // spawn all items from scenario
        for (const auto &def: scenario.entities) {
            Factories::createUnit(registry, def);
        }

    }

    void update() {
        const auto player_entity = registry.ctx().get<PlayerEntity>().id;
        if (registry.all_of<Crashed>(player_entity)) return;

        const auto dt = GetFrameTime();
        dispatcher.update(registry, dt);
        WidgetsInputs(registry);
    }

    void draw() {
        ClearBackground(scenario.skyColor);

        // 3D
        BeginMode3D(dispatcher.playerCamera.getCamera());
        WorldStreamerSystem(registry);
        RenderModels(registry);
        RenderDebugging(registry);
        EndMode3D();

        // 2D
        RenderCockpit(registry);
        RenderMinimap(registry);
        RenderEngineStatus(registry);
        RenderHud(registry);
        RenderRadar(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
