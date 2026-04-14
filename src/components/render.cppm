module;
#include <entt/entt.hpp>
// #include <raylib.h>

export module RenderComponents;

import RaylibResource;
import ResourceManager;


export struct Modeled {
    entt::resource<ModelResource> handle;
};

export struct Imaged {
    entt::resource<ImageResource> handle;
};

// export struct Textured {
//     TextureHandle texture;
// };
//
// export struct Shaded {
//     ShaderHandle shader;
// };
//
