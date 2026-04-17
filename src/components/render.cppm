module;
#include <entt/entt.hpp>

export module Components:Render;

import RaylibResource;
import ResourceManager;

// resources

export struct Modeled {
    entt::resource<ModelResource> handle;
};

export struct Imaged {
    entt::resource<ImageResource> handle;
};

export struct Textured {
    // entt::resource<TextureResource> handle;
};

// widgets tags

export struct MinimapWidget {
};

export struct RadarWidget {
};

export struct EngineWidget {
};
