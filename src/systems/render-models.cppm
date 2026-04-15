module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderSystem:Models;

import Components;

export void RenderModels(entt::registry &registry) {
    const auto view = registry.view<Position3D, Modeled>(entt::exclude<World>);
    const auto offset = registry.ctx().get<Offset>().offset;

    for (auto [entity, position, modeled]: view.each()) {
        const Model& actualModel = modeled.handle->model;
        DrawModel(actualModel, position.pos - offset, 1.0f, WHITE);
    }
}
