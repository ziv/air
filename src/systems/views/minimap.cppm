module;
#include "../../lib/ray.hpp"
#include <entt/entt.hpp>

export module Views:Minimap;

import Types;
import Helpers;
import Components;
import RaylibResource;
import :Base;

struct MinimapConfig {
    std::string mapTexture; ///< Path to the satellite map texture.
    Pixel x = 10; ///< X position of the minimap on the screen.
    Pixel y = 10; ///< Y position of the minimap on the screen.
    Pixel size = 150; ///< Size (width and height) of the square minimap widget.
    Ratio mapsRatio = 62.5f; ///< Ratio between the world coordinates and the map texture coordinates (e.g., 1 pixel on the map represents 62.5 units in the world).
};

export class MinimapView : public ViewBase {
    TextureHandle tex; ///< Satellite map texture.
    Camera2D mapCamera = {0}; ///< 2D camera centred on the aircraft's map position.
    float heading = 0.0f; ///< Current compass heading for the aircraft icon.
    float zoom = 1.0; ///< Map zoom level (adjustable with Z/X).

public:
    // explicit MinimapView()
    void update(entt::registry &registry, const float dt) override {
        const auto entity = registry.ctx().get<PlayerEntity>().id;
        const auto player = registry.get<Player>(entity);

        if (IsKeyPressed(KEY_Z)) zoom += 0.5f;
        if (IsKeyPressed(KEY_X)) zoom -= 0.5f;
        if (zoom > 5.0f) zoom = 5.0f;
        if (zoom < 0.5f) zoom = 0.5f;

        const auto aircraftPosition = player.pos - player.offset;
        const float mapX = aircraftPosition.x / 62.5f; // todo from conf
        const float mapZ = aircraftPosition.z / 62.5f;

        mapCamera.target = (Vector2){mapX, mapZ};
        mapCamera.offset = (Vector2){75.0, 75.0f}; // half widget size, from conf
        mapCamera.rotation = 0.0f;
        mapCamera.zoom = zoom;

        const Vector3 forward = player.forward;
        heading = 180.0f - atan2f(forward.x, forward.z) * RAD2DEG;
    }

    void draw(entt::registry &registry) override {
    }
};
