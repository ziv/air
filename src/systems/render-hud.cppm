module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"
#include <rlgl.h>

export module RenderHudLadder;

import AircraftComponents;
import WorldComponents;

export void RenderHudLadder(entt::registry &registry) {
    // constexpr int ladderX = 440;
    // constexpr int ladderY = 200;
    // constexpr int ladderWidth = 320;
    // constexpr int ladderHeight = 220;
    // constexpr float fov = 80.0f;
    // constexpr int width = 1200;
    // constexpr int height = 800;
    // constexpr float ladderOffset = 0.0f;
    //
    // auto view = registry.view<Player, Orientation>();
    //
    // for (auto [entity, orientation]: view.each()) {
    //     BeginScissorMode(ladderX, ladderY, ladderWidth, ladderHeight);
    //
    //     const auto fy = orientation.forward.y;
    //     const auto uy = orientation.up.y;
    //     const auto [x, y, z] = orientation.right;
    //
    //     const auto pitch = asinf(fy) * RAD2DEG;
    //     const auto roll = (fabsf(fy) < 0.999f ? atan2f(-y, uy) : atan2f(z, x)) * RAD2DEG;
    //
    //     const auto centerX = width / 2;
    //     const auto centerY = height / 2;
    //
    //     const auto pixelsPerDegree = static_cast<float>(height) / fov;
    //
    //     DrawCircleLines(centerX, centerY - static_cast<int>(ladderOffset), 5.0f, GREEN);
    //
    //     // freeze state
    //     rlPushMatrix();
    //
    //     // take us to the center
    //     rlTranslatef(static_cast<float>(centerX), static_cast<float>(centerY) - ladderOffset, 0);
    //
    //     // pitch & roll
    //     rlRotatef(-roll, 0, 0, 1);
    //     rlTranslatef(0, pitch * pixelsPerDegree, 0);
    //
    //     // draw on the center
    //
    //     // main line
    //     DrawLineEx({-100, 0}, {-20, 0}, 2, GREEN);
    //     DrawLineEx({20, 0}, {100, 0}, 2, GREEN);
    //
    //
    //     for (int i = -180; i <= 180; i += 15) {
    //         if (i == 0) continue;
    //         const auto lineY = -static_cast<float>(i) * pixelsPerDegree;
    //         // main line
    //         DrawLineEx({-100, lineY}, {-20, lineY}, 1, GREEN);
    //         DrawLineEx({20, lineY}, {100, lineY}, 1, GREEN);
    //
    //         const auto to = i > 0 ? lineY + 10 : lineY - 10;
    //         DrawLineEx({100, lineY}, {110, to}, 1, GREEN);
    //         DrawLineEx({-100, lineY}, {-110, to}, 1, GREEN);
    //         DrawText(TextFormat("%d", i), -130, static_cast<int>(lineY) - 5, 10, GREEN);
    //         DrawText(TextFormat("%d", i), 115, static_cast<int>(lineY) - 5, 10, GREEN);
    //     }
    //
    //     // resume from freeze
    //     rlPopMatrix();
    //     EndScissorMode();
    // }
}
