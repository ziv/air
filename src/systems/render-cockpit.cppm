module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module ChromaRender;

import AircraftComponents;
import WorldComponents;

export void RenderChroma(entt::registry &registry) {
    registry.sort<ChromaRender>([](const auto &lhs, const auto &rhs) {
        return lhs.priority < rhs.priority;
    });

    auto view = registry.view<Position2D, ChromaRender>();

    for (auto [entity, position, chroma]: view.each()) {
        BeginShaderMode(chroma.shader);
        DrawTextureV(chroma.tex, position.pos, chroma.tint);
        EndShaderMode();
    }
    // view.use<ChromaRender>().each([](const auto &pos, const auto &render) {
    //     BeginShaderMode(render.shader);
    //     DrawTextureV(render.texture, pos.pos, render.tint);
    //     EndShaderMode();
    // });
}
