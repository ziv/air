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
    // drag
    float cd;
    float inducedDragCoefficient;

    // lift
    float cl;
    float liftSlopeCoefficient;
    float stallAngle;

    // weight
    float weight;

    // agility
    float pitchRatio;
    float rollRatio;
    float yawRatio;
};

export struct AircraftUtils {
    bool brake; // aircraft brakes status
    bool gear; // aircraft landing gear status
};

export struct Engine {
    float thrust;
    float throttle;
};

export struct AircraftControls {
    float roll;
    float pitch;
    float yaw;
    bool brake; // pressed or not
    bool gear; // pressed or not
    float throttle;
    bool afterBurner;
};

export struct Grounded {
    bool grounded;
};