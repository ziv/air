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
import Views;


export class Game {
    entt::registry &registry;
    Scenario scenario;

    PlayerDispatcher dispatcher;
    std::vector<std::unique_ptr<ViewBase> > views;

public:
    explicit Game(const JsonConfig &cfg,
                  const JsonConfig &scn,
                  entt::registry &reg)
        : registry(reg),
          scenario(scn.get<Scenario>("/data")),
          dispatcher(cfg) {
        // set initial offset
        registry.ctx().emplace<Offset>(Vector3Zero());

        views.push_back(std::make_unique<CockpitView>(cfg.get<CockpitConfig>("/views/cockpit")));
        views.push_back(std::make_unique<HudView>(cfg.get<GlobalConfig>("/global"), cfg.get<HudConfig>("/views/hud")));
        views.push_back(std::make_unique<MinimapView>(cfg.get<MinimapConfig>("/views/minimap")));
        views.push_back(std::make_unique<EngineView>());

        Factories::createPlayer(registry, cfg, scenario);
        Factories::createScene(registry, cfg);
        Factories::createCockpitWidgets(registry, cfg);

        // spawn all items from scenario
        for (const auto &def: scenario.entities) {
            Factories::createUnit(registry, def);
        }
    }

    void update() {
        const float dt = GetFrameTime();

        const auto id = registry.ctx().get<PlayerEntity>().id;
        if (registry.all_of<Crashed>(id)) return;

        dispatcher.update(registry, dt);
        for (const auto &v: views) v->update(registry, dt);
    }

    void draw() {
        ClearBackground(scenario.skyColor);
        // ClearBackground(BLUE);

        // 3D
        BeginMode3D(dispatcher.playerCamera.getCamera());
        WorldStreamerSystem(registry);
        RenderModels(registry);
        RenderDebugging(registry);
        EndMode3D();

        // 2D
        // RenderCockpit(registry);
        for (const auto &v: views) v->draw(registry);
        DrawFPS(1050, 780);
        RenderDebug(registry);
    }
};
