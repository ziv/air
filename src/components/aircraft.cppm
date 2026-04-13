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


export struct Aircraft {
    float cd;
    float cl;
    float weight;
};

export struct Engine {
    float thrust;
    float throttle;
};

export struct StickInputs {
    float roll;
    float pitch;
    float yaw;
};
