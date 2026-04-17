module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"
#include <rlgl.h>

export module RenderSystem:Hud;

import Components;
import Helpers;

void draw_ladder(const HudWidget &widget, const Player &player) {
    const auto fy = player.forward.y;
    const auto uy = player.up.y;
    const auto [x, y, z] = player.right;

    const auto pitch = asinf(fy) * RAD2DEG;
    const auto roll = (fabsf(fy) < 0.999f ? atan2f(-y, uy) : atan2f(z, x)) * RAD2DEG;

    BeginScissorMode(widget.cfg.ladder.x, widget.cfg.ladder.y, widget.cfg.ladder.width, widget.cfg.ladder.height);
    DrawCircleLines(widget.centerX, widget.centerY - widget.cfg.ladder.offset, 5.0f, GREEN);

    // freeze state
    rlPushMatrix();

    // take us to the center
    // the offset allow to define where to put the 0 line
    rlTranslatef(
        static_cast<float>(widget.centerX),
        static_cast<float>(widget.centerY) - static_cast<float>(widget.cfg.ladder.offset),
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

void draw_warnings(const HudWidget &widget, const PlayerInputs &inputs) {
    // after burner warning
    if (inputs.throttle > 1.0f) {
        DrawText("A/B ON", widget.cfg.warnings.x, widget.cfg.warnings.y, widget.cfg.warnings.font, ORANGE);
    }

    // autopilot warning
    if (inputs.autopilot) {
        DrawText("A/P ON", widget.cfg.warnings.x, widget.cfg.warnings.y, widget.cfg.warnings.font, GREEN);
    }
}

void draw_heading(const HudWidget &widget, const Player &player) {
    const auto [x, y, width, font] = widget.cfg.heading;
    const auto fx = static_cast<float>(x);
    // constexpr auto pixelsPerDegree = 10.0f;
    constexpr auto tickInterval = 5;

    auto currentHeading = std::atan2(-player.forward.x, player.forward.z) * RAD2DEG;
    // apply 180-degree offset to align north (0) with the world's 180 degree mark.
    // since atan2 returns [-180, 180], adding 180 shifts the range to [0, 360].
    currentHeading += 180.0f;

    // normalize 360 to 0
    if (currentHeading >= 360.0f) {
        currentHeading -= 360.0f;
    }

    DrawLine(x, y, x, y + font / 2, GREEN);
    DrawText(TextFormat("%03.0f", currentHeading),
             x - font / 2,
             y - font,
             font,
             GREEN);

    const auto halfWidth = width / 2;
    const auto halfWidth_f = static_cast<float>(halfWidth);
    DrawLine(x - halfWidth, y, x + halfWidth, y, GREEN);

    for (auto i = 0; i < 360; i += tickInterval) {
        // calculate shortest angular difference between current heading and the tick
        auto diff = static_cast<float>(i) - currentHeading;

        // normalize difference to [-180, 180] to handle the 360->0 wrap around
        if (diff > 180.0f) diff -= 360.0f;
        else if (diff < -180.0f) diff += 360.0f;

        // calculate screen X position and check if it's within the HUD tape width using if-init
        if (const auto tickX = fx + (diff * widget.ppd); tickX >= fx - halfWidth_f && tickX <= fx + halfWidth_f) {
            const auto isMajorTick = (i % 10 == 0);
            const auto tickLength = isMajorTick ? font / 2 : font / 4;

            // Draw the tick line
            DrawLine(static_cast<int>(tickX), y,
                     static_cast<int>(tickX), y + tickLength,
                     GREEN);

            // Draw text for major ticks (every 10 degrees)
            if (isMajorTick) {
                const auto *text = TextFormat("%03d", i);
                const auto textOffset = static_cast<float>(font) * 0.6f;

                DrawText(text,
                         static_cast<int>(tickX - textOffset),
                         y + tickLength + 4,
                         static_cast<int>(static_cast<float>(font) * 0.8f),
                         GREEN);
            }
        }
    }
}

void draw_boresight(const HudWidget &widget) {
    const auto [x, y, size] = widget.cfg.boresight;
    const auto left = x - size / 2;
    const auto right = x + size / 2;
    const auto part = size / 4;
    const auto tip = size / 8;

    const auto noseY = y - static_cast<int>(widget.tilt * RAD2DEG * widget.ppd);
    // let line
    DrawLine(left, noseY, left + part, noseY, GREEN);
    // right line
    DrawLine(right - part, noseY, right, noseY, GREEN);

    // W
    DrawLine(left + part, noseY, left + part + tip, noseY + tip, GREEN);
    DrawLine(left + part + tip, noseY + tip, x, noseY, GREEN);
    DrawLine(x, noseY, right - part - tip, noseY + tip, GREEN);
    DrawLine(right - part - tip, noseY + tip, right - part, noseY, GREEN);
}

void draw_height_indicator(const HudWidget &widget, const Player &player) {
    const auto heightAbsolute = meter_to_feet(player.pos.y);
    DrawText(TextFormat("%s", number_suffix(heightAbsolute)),
             widget.cfg.height.x,
             widget.cfg.height.y,
             widget.cfg.height.font,
             GREEN);
    // DrawRectangleLines(745, 328, 30, 14, colors[color]);
}

void draw_speed_indicator(const HudWidget &widget, const Player &player) {
    const auto speed = ms_to_knots(player.speed);
    DrawText(TextFormat("%s", number_suffix(speed)),
             widget.cfg.speedometer.x,
             widget.cfg.speedometer.y,
             widget.cfg.speedometer.font,
             GREEN);
}

void draw_rate_of_climb(const HudWidget &widget, const Player &player) {
    const auto verticalSpeedFPM = ms_to_fpm(player.velocity.y);
    constexpr float MAX_CLIMB_RATE_FPM = 50000.0f;
    const float MAX_BAR_PIXELS = (static_cast<float>(widget.cfg.roc.height) / 2.0f) - 20.0f;

    float vsRatio = verticalSpeedFPM / MAX_CLIMB_RATE_FPM;
    if (vsRatio > 1.0f) vsRatio = 1.0f;
    if (vsRatio < -1.0f) vsRatio = -1.0f;

    const int currentBarHeight = static_cast<int>(vsRatio * MAX_BAR_PIXELS);
    const int maxBarPixels = static_cast<int>(MAX_BAR_PIXELS);

    const int centerX = widget.cfg.roc.x; // see speed location
    const int centerY = widget.cfg.roc.y;
    const int width = widget.cfg.roc.width;
    DrawLine(centerX, centerY - maxBarPixels, centerX, centerY + maxBarPixels, Fade(GREEN, 0.3f));
    DrawLine(centerX - width, centerY, centerX + width, centerY, GREEN);

    if (currentBarHeight > 0) {
        DrawRectangle(centerX - width / 2, centerY - currentBarHeight, width, currentBarHeight, GREEN);
    } else {
        DrawRectangle(centerX - width / 2, centerY, width, -currentBarHeight, GREEN);
    }
}

export void RenderHud(entt::registry &registry) {
    const auto view = registry.view<HudWidget>();
    if (view.begin() == view.end()) return;

    const auto entity = view.front();
    const auto hud = registry.get<HudWidget>(entity);

    const auto player_entity = registry.ctx().get<PlayerEntity>().id;
    const auto [player, inputs] = registry.get<Player, PlayerInputs>(player_entity);

    draw_ladder(hud, player);
    draw_rate_of_climb(hud, player);
    draw_speed_indicator(hud, player);
    draw_height_indicator(hud, player);
    draw_boresight(hud);
    draw_heading(hud, player);
    draw_warnings(hud, inputs);
}
