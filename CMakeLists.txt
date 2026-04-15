cmake_minimum_required(VERSION 4.2.2)
project(airborne LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch raylib from GitHub
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# Fetch entt from GitHub
include(FetchContent)
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.16.0
)
FetchContent_MakeAvailable(entt)

# Fetch nlohmann_json from GitHub
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES FILES
        src/Game.cppm
        src/player/player.cppm
        src/player/player-camera.cppm
        src/player/player-config.cppm
        src/player/player-controls.cppm
        src/player/player-position.cppm
        src/player/player-physics.cppm
        src/player/player-rotation.cppm
        src/core/types.cppm
        src/core/scenario-types.cppm
        src/core/units-types.cppm
        src/core/helpers.cppm
        src/core/JsonConfig.cppm
        src/core/config-types.cppm
        src/core/Resource.cppm
        src/core/ResourceManager.cppm
        src/core/Constants.cppm
        src/components/world.cppm
        src/components/aircraft.cppm
        src/components/render.cppm
        src/prefabs/create_player.cppm
        src/prefabs/create_scene.cppm
        src/prefabs/create-cockpit.cppm
        src/systems/aircraft-physics.cppm
        src/systems/aircraft-mechanics.cppm
        src/systems/render-cockpit.cppm
        src/systems/render-models.cppm
        src/systems/render-debug.cppm
        src/systems/render-hud.cppm
        src/systems/world-streamer.cppm
        src/systems/position.cppm
        src/systems/velocity.cppm
        src/systems/user-inputs.cppm
        src/systems/rotation.cppm
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib EnTT::EnTT nlohmann_json::nlohmann_json)
#target_compile_options(${PROJECT_NAME} PRIVATE
#        -Wall
#        -Wextra
#        -Wpedantic
#)