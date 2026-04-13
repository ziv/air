module;
#include <raylib.h>

export module AircraftComponents;

import RaylibResource;

export struct Cockpit {
};

export struct ChromaRender {
    TextureHandle tex;
    ShaderHandle shader;
    Color tint = WHITE;
    int priority = 0;
};
