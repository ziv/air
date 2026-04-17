module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"
#include <rlgl.h>

export module RenderSystem:Hud;

import Components;

void draw_ladder(const Player &player, const HudWidget &widget) {
    const auto conf = widget.cfg;
    const auto centerX = widget.centerX;
    const auto centerY = widget.centerY;
    const auto fy = player.forward.y;
    const auto uy = player.up.y;
    const auto [x, y, z] = player.right;

    const auto pitch = asinf(fy) * RAD2DEG;
    const auto roll = (fabsf(fy) < 0.999f ? atan2f(-y, uy) : atan2f(z, x)) * RAD2DEG;

    BeginScissorMode(conf.ladder.x, conf.ladder.y, conf.ladder.width, conf.ladder.height);
    DrawCircleLines(centerX, centerY - conf.ladder.offset, 5.0f, GREEN);

    // freeze state
    rlPushMatrix();

    // take us to the center
    // the offset allow to define where to put the 0 line
    rlTranslatef(
        static_cast<float>(centerX),
        static_cast<float>(centerY) - static_cast<float>(conf.ladder.offset),
        0);

    // pitch & roll
    rlRotatef(-roll, 0, 0, 1);
    rlTranslatef(0, pitch * widget.ppd, 0);

    // main line
    DrawLineEx({-100, 0}, {-20, 0}, 2, GREEN);
    DrawLineEx({20, 0}, {100, 0}, 2, GREEN);

    for (int i = -180; i <= 180; i += 15) {
        if (i == 0) continue;
        const auto lineY = -static_cast<float>(i * widget.pixelsPerDegree);
        const auto lineYint = static_cast<int>(lineY);
        // main line
        DrawLineEx({-100, lineY}, {-20, lineY}, 1, GREEN);
        DrawLineEx({20, lineY}, {100, lineY}, 1, GREEN);

        // wings
        const auto to = i > 0 ? lineY + 10 : lineY - 10;
        DrawLineEx({100, lineY}, {110, to}, 1, GREEN);
        DrawLineEx({-100, lineY}, {-110, to}, 1, GREEN);
        DrawText(TextFormat("%d", i), -130, lineYint - 5, 10, GREEN);
        DrawText(TextFormat("%d", i), 115, lineYint - 5, 10, GREEN);
    }

    // resume from freeze
    rlPopMatrix();
    EndScissorMode();
}

export void RenderHud(entt::registry &registry) {
    const auto view = registry.view<HudWidget>();
    if (view.begin() == view.end()) return;

    const auto entity = view.front();
    const auto hud = registry.get<HudWidget>(entity);

    const auto player_entity = registry.ctx().get<PlayerEntity>().id;
    const auto player = registry.get<Player>(player_entity);


    draw_ladder(player, hud);
    // drawRateOfClimb(player);
    // speedIndicator(player);
    // heightIndicator(player);
    // boresight();
    // heading(player);
    // warnings(registry.get<PlayerInputs>(entity));
}
