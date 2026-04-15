module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderSystem:Models;

import Components;

export void RenderModel(entt::registry &registry) {
    auto view = registry.view<Position3D, Modeled>(entt::exclude<World>);
    auto offset = registry.ctx().get<Offset>().offset;

    for (auto [entity, position, modeled]: view.each()) {
        const Model& actualModel = modeled.handle->model;
        const Vector3 drawPosition = position.pos - offset;
        DrawModel(actualModel, drawPosition, 1.0f, WHITE);
        DrawCubeWires(drawPosition, 50.0f, 50.0f, 50.0f, YELLOW);
    }
}
