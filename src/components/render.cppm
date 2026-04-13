module;
// #include <entt/entt.hpp>
// #include <raymath.h>

export module RendeComponents;

import RaylibResource;

export struct Modeled {
    ModelHandle model;
};

export struct Textured {
    TextureHandle texture;
};

export struct Shaded {
    ShaderHandle shader;
};