module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"

export module RenderSystem:Models;

import Components;

export void RenderModels(entt::registry &registry) {
    // const auto view = registry.view<Position3D, Modeled, Heading>(entt::exclude<World>);
    // const auto offset = registry.ctx().get<Offset>().offset;
    //
    // for (auto [entity, position, modeled, heading]: view.each()) {
    //     const auto p = position.pos + offset;
    //     // DrawModelEx(modeled.handle->model, p, {0.0f, 1.0f, 0.0f}, heading.heading, {1.0f, 1.0f, 1.0f}, WHITE);
    // }
}

// export void RenderHeadingModels(entt::registry &registry) {
//     const auto view = registry.view<Position3D, Modeled, Heading>(entt::exclude<World>);
//     const auto offset = registry.ctx().get<Offset>().offset;
//
//     for (auto [entity, position, modeled, heading]: view.each()) {
//         const auto p = position.pos + offset;
//         DrawModelEx(modeled.handle->model, p, {0.0f, 1.0f, 0.0f}, heading.heading, {1.0f, 1.0f, 1.0f}, WHITE);
//     }
// }
