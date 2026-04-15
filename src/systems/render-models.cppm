module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderSystem:Models;

import Components;

export void RenderModels(entt::registry &registry) {
    const auto view = registry.view<Position3D, Modeled, Identify>(entt::exclude<World>);
    const auto offset = registry.ctx().get<Offset>().offset;

    for (auto [entity, position, modeled, id]: view.each()) {
        const auto p = position.pos + offset;
        DrawModel(modeled.handle->model, p, 1.0f, WHITE);
    }
}
