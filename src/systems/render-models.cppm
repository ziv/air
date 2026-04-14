module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderModel;

import WorldComponents;
import RenderComponents;

export void RenderModel(entt::registry &registry) {
    auto view = registry.view<Position3D, Modeled>();

    for (auto [entity, position, modeled]: view.each()) {
        const Model& actualModel = modeled.handle->model;
        DrawModel(actualModel, position.pos, 1.0f, WHITE);
    }
}
