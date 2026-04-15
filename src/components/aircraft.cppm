module;
#include <raylib.h>

export module Components:Aircraft;

import RaylibResource;

// area we can land on
export struct Landable {
    bool carrier;
};


export struct Heading {
    float heading;
};

export struct Crashed {
    bool crashed;
};

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

// events/states

export struct Grounded {
};

export struct Flying {
};

export struct TouchDown {
};

export struct LandingZone {
    bool isLandingZone;
    bool isCarrier;
    float surfaceY;
};