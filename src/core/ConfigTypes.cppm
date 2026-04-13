module;
#include <nlohmann/json.hpp>
#include <string>

export module ConfigTypes;

export struct WindowConfig {
    std::string title;
    int width;
    int height;
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowConfig, title, width, height);
}
