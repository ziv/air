module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"
#include <rlgl.h>

export module Views:Hud;

import Types;
import Helpers;
import Components;
import :Base;

export struct HudLadderConfig {
    Pixel x = 440;
    Pixel y = 200;
    Pixel width = 320;
    Pixel height = 220;
    Pixel offset = 0;
};

export struct RocConfig {
    Pixel x;
    Pixel y;
    Pixel width;
    Pixel height;
};

export struct SpeedometerConfig {
    Pixel x;
    Pixel y;
    Size font;
};

export struct HeightIndicatorConfig {
    Pixel x;
    Pixel y;
    Size font;
};

export struct BoresightConfig {
    Pixel x;
    Pixel y;
    Size size;
};


export struct HeadingConfig {
    Pixel x;
    Pixel y;
    Pixel width;
    Size font;
};

export struct WarningsConfig {
    Pixel x;
    Pixel y;
    Size font;
};

export struct HudConfig {
    HudLadderConfig ladder;
    RocConfig roc;
    SpeedometerConfig speedometer;
    HeightIndicatorConfig height;
    BoresightConfig boresight;
    HeadingConfig heading;
    WarningsConfig warnings;
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HudLadderConfig, x, y, width, height, offset);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RocConfig, x, y, width, height);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpeedometerConfig, x, y, font);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HeightIndicatorConfig, x, y, font);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BoresightConfig, x, y, size);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HeadingConfig, x, y, width, font);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WarningsConfig, x, y, font);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HudConfig, ladder, roc, speedometer, height, boresight, heading, warnings);
}

export class HudView : public ViewBase {
    GlobalConfig gc;
    HudConfig config;

    Pixel centerX;
    Pixel centerY;
    Pixel pixelsPerDegree;

public:
    explicit HudView(const GlobalConfig &g, const HudConfig &c) : gc(g), config(c) {
        centerX = gc.width / 2;
        centerY = gc.height / 2;
        pixelsPerDegree = static_cast<float>(gc.height) / gc.fov;
    }

    void update(entt::registry &registry, const float dt) override {
        // default: no per-frame behavior
        // listen to ALT+H / OPTION+H to change HUD color
    }

    void draw(entt::registry &registry) override {
        const auto entity = registry.ctx().get<PlayerEntity>().id;
        const auto player = registry.get<Player>(entity);
        drawLadder(player);
        drawRateOfClimb(player);
        speedIndicator(player);
        heightIndicator(player);
        boresight();
        heading(player);
        warnings(registry.get<PlayerInputs>(entity));
    }

private:
    void warnings(const PlayerInputs &inputs) const {
        // after burner warning
        if (inputs.throttle > 1.0f) {
            DrawText("A/B ON", config.warnings.x, config.warnings.y, config.warnings.font, ORANGE);
        }

        // autopilot warning
        if (inputs.autopilot) {
            DrawText("A/P ON", config.warnings.x, config.warnings.y, config.warnings.font, GREEN);
        }
    }

    void heading(const Player &player) const {
        const auto [x, y, width, font] = config.heading;

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
                 x - font / 2.0f,
                 y - font,
                 font,
                 GREEN);

        const auto halfWidth = width / 2.0f;
        DrawLine(x - halfWidth, y, x + halfWidth, y, GREEN);

        for (auto i = 0; i < 360; i += tickInterval) {
            // calculate shortest angular difference between current heading and the tick
            auto diff = static_cast<float>(i) - currentHeading;

            // normalize difference to [-180, 180] to handle the 360->0 wrap around
            if (diff > 180.0f) diff -= 360.0f;
            else if (diff < -180.0f) diff += 360.0f;

            // calculate screen X position and check if it's within the HUD tape width using if-init
            if (const auto tickX = x + (diff * pixelsPerDegree); tickX >= x - halfWidth && tickX <= x + halfWidth) {
                const auto isMajorTick = (i % 10 == 0);
                const auto tickLength = static_cast<int>(isMajorTick ? (font / 2.0f) : (font / 4.0f));

                // Draw the tick line
                DrawLine(static_cast<int>(tickX), y,
                         static_cast<int>(tickX), y + tickLength,
                         GREEN);

                // Draw text for major ticks (every 10 degrees)
                if (isMajorTick) {
                    const auto *text = TextFormat("%03d", i);
                    const auto textOffset = font * 0.6f;

                    DrawText(text,
                             static_cast<int>(tickX - textOffset),
                             static_cast<int>(y + tickLength + 4.0f),
                             static_cast<int>(font * 0.8f),
                             GREEN);
                }
            }
        }
    }

    void boresight() const {
        const auto [x, y, size] = config.boresight;
        const auto left = x - size / 2;
        const auto right = x + size / 2;
        const auto part = size / 4;
        const auto tip = size / 8;

        const auto noseY = y - static_cast<int>(gc.tilt * RAD2DEG * pixelsPerDegree);
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

    void heightIndicator(const Player &player) const {
        const auto heightAbsolute = meter2feet(player.pos.y);
        DrawText(TextFormat("%s", numberSuffix(heightAbsolute)),
                 config.height.x,
                 config.height.y,
                 config.height.font,
                 GREEN);
        // DrawRectangleLines(745, 328, 30, 14, colors[color]);
    }

    void speedIndicator(const Player &player) const {
        const auto speed = msToKnots(player.speed);
        DrawText(TextFormat("%s", numberSuffix(speed)),
                 config.speedometer.x,
                 config.speedometer.y,
                 config.speedometer.font,
                 GREEN);
    }

    void drawLadder(const Player &player) const {
        const auto fy = player.forward.y;
        const auto uy = player.up.y;
        const auto [x, y, z] = player.right;

        const auto pitch = asinf(fy) * RAD2DEG;
        const auto roll = (fabsf(fy) < 0.999f ? atan2f(-y, uy) : atan2f(z, x)) * RAD2DEG;

        BeginScissorMode(config.ladder.x, config.ladder.y, config.ladder.width, config.ladder.height);
        DrawCircleLines(centerX, centerY - config.ladder.offset, 5.0f, GREEN);

        // freeze state
        rlPushMatrix();

        // take us to the center
        // the offset allow to define where to put the 0 line
        rlTranslatef(
            static_cast<float>(centerX),
            static_cast<float>(centerY) - static_cast<float>(config.ladder.offset),
            0);

        // pitch & roll
        rlRotatef(-roll, 0, 0, 1);
        rlTranslatef(0, pitch * pixelsPerDegree, 0);

        // main line
        DrawLineEx({-100, 0}, {-20, 0}, 2, GREEN);
        DrawLineEx({20, 0}, {100, 0}, 2, GREEN);

        for (int i = -180; i <= 180; i += 15) {
            if (i == 0) continue;
            const auto lineY = -static_cast<float>(i) * pixelsPerDegree;
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


    void drawRateOfClimb(const Player &player) const {
        const auto verticalSpeedFPM = msToFpm(player.velocity.y);
        constexpr float MAX_CLIMB_RATE_FPM = 50000.0f;
        const float MAX_BAR_PIXELS = (static_cast<float>(config.roc.height) / 2.0f) - 20.0f;

        float vsRatio = verticalSpeedFPM / MAX_CLIMB_RATE_FPM;
        if (vsRatio > 1.0f) vsRatio = 1.0f;
        if (vsRatio < -1.0f) vsRatio = -1.0f;

        const int currentBarHeight = static_cast<int>(vsRatio * MAX_BAR_PIXELS);
        const int maxBarPixels = static_cast<int>(MAX_BAR_PIXELS);

        const int centerX = config.roc.x; // see speed location
        const int centerY = config.roc.y;
        const int width = config.roc.width;
        DrawLine(centerX, centerY - maxBarPixels, centerX, centerY + maxBarPixels, Fade(GREEN, 0.3f));
        DrawLine(centerX - width, centerY, centerX + width, centerY, GREEN);

        if (currentBarHeight > 0) {
            DrawRectangle(centerX - width / 2, centerY - currentBarHeight, width, currentBarHeight, GREEN);
        } else {
            DrawRectangle(centerX - width / 2, centerY, width, -currentBarHeight, GREEN);
        }
    }
};
