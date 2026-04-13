module;
#include <nlohmann/json.hpp>
#include <string>
#include "../lib/ray.hpp"

export module ConfigTypes;

export struct WindowConfig {
    std::string title;
    int width;
    int height;
};


export struct PilotConfig {
    float fov;
    float tilt;
};

export struct CockpitConfig {
    std::string texturePath;
    std::string shaderPath;
    Color tintColor{};
};

export struct AircraftConfig {
    float weight;
    float engineThrust;
    float vleSpeed;
    float liftCoefficient;
    float liftSlopeCoefficient;
    float stallAngle;
    float dragCoefficient;
    float inducedDragCoefficient;
    float pitchRatio;
    float rollRatio;
    float yawRatio;
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowConfig, title, width, height);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PilotConfig, fov, tilt);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CockpitConfig, texturePath, shaderPath, tintColor);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AircraftConfig, weight, engineThrust, vleSpeed, liftCoefficient, liftSlopeCoefficient, stallAngle, dragCoefficient, inducedDragCoefficient, pitchRatio, rollRatio, yawRatio);
}
