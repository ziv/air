module;
#include <entt/entt.hpp>
#include "../../lib/ray.hpp"

export module RenderSystem:Cockpit;

import Components;

export void RenderCockpit(entt::registry &registry) {
    auto view = registry.view<Position2D, ChromaRender>();

    for (auto [entity, position, chroma]: view.each()) {
        BeginShaderMode(chroma.shader);
        DrawTextureV(chroma.tex, position.pos, chroma.tint);
        EndShaderMode();
    }
}
