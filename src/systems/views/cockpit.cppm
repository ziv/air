module;
#include "../../lib/ray.hpp"
#include <entt/entt.hpp>

export module Views:Cockpit;

import Types;
import Helpers;
import Components;
import RaylibResource;
import :Base;

export class CockpitView : public ViewBase {
    CockpitConfig conf;
    TextureHandle tex;
    ShaderHandle shader;

public:
    explicit CockpitView(const CockpitConfig &c)
        : conf(c),
          tex(LoadTexture(c.texturePath.c_str())),
          shader(LoadShader(nullptr, c.shaderPath.c_str())) {
    }

    void draw(entt::registry &registry) override {
        BeginShaderMode(shader);
        DrawTextureV(tex, (Vector2){0.0f, 0.0f}, conf.tintColor);
        EndShaderMode();
    }
};
