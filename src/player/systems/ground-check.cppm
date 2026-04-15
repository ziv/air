module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"

export module Player:GroundCheck;

import Components;

export void PlayerGroundCheck(entt::registry &registry, float stallSpeed = 65.0f) {
    const auto view = registry.view<Player, TouchDown>();

    for (auto [entity, player]: view.each()) {
        const float fwdY = std::clamp(player.forward.y, -1.0f, 1.0f);
        const float rightY = std::clamp(player.right.y, -1.0f, 1.0f);

        const float pitchDeg = asinf(fwdY) * RAD2DEG;
        const float rollDeg = asinf(rightY) * RAD2DEG;

        const bool wingsLevel = fabsf(rollDeg) < 10.0f;
        const bool noseOk = pitchDeg > -5.0f && pitchDeg < 20.0f;
        const bool slowEnough = player.speed < stallSpeed * stallSpeed * 2.0f;

        // todo check sink rate

        if (!wingsLevel) {
            TraceLog(LOG_WARNING, "Landing failed: wings not level (roll = %.1f°)", rollDeg);
        }
        if (!noseOk) {
            TraceLog(LOG_WARNING, "Landing failed: bad pitch angle (pitch = %.1f°)", pitchDeg);
        }
        if (!slowEnough) {
            TraceLog(LOG_WARNING, "Landing failed: too fast (speed = %.1f m/s)", player.speed);
        }
        if (!wingsLevel && !noseOk && !slowEnough) {
            if (registry.all_of<Crashed>(entity)) {
                registry.emplace<Crashed>(entity);
                TraceLog(LOG_WARNING, "PlayerGroundCheck: aircraft crashed on landing");
            }
        }
    }

    // remove the touchdown tag
    const auto entity = registry.ctx().get<PlayerEntity>().id;
    registry.erase<TouchDown>(entity);
    TraceLog(LOG_DEBUG, "PlayerGroundCheck: removed TouchDown tag");
}
