module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderSystem:ViewDebugging;

import Components;

export void RenderDebugging(entt::registry &registry) {
    auto view = registry.view<Position3D>(entt::exclude<World>);
    auto offset = registry.ctx().get<Offset>().offset;

    for (auto [entity, position]: view.each()) {
        const Vector3 drawPosition = position.pos - offset;
        DrawCubeWires(drawPosition, 50.0f, 50.0f, 50.0f, YELLOW);
    }
}
