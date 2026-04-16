module;
#include "../../lib/ray.hpp"
#include <entt/entt.hpp>

export module Views:Minimap;

import Types;
import Helpers;
import Components;
import RaylibResource;
import :Base;

export struct MinimapConfig {
    std::string mapTexture; ///< Path to the satellite map texture.
    Pixel size = 150; ///< Size (width and height) of the square minimap widget.
    Ratio mapsRatio = 62.5f; ///< Ratio between the world coordinates and the map texture coordinates
};

export
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MinimapConfig, mapTexture, size, mapsRatio);
}

export class MinimapView : public ViewBase {
    MinimapConfig conf;
    TextureHandle tex; ///< Satellite map texture.
    Camera2D mapCamera = {0}; ///< 2D camera centred on the aircraft's map position.
    float heading = 0.0f; ///< Current compass heading for the aircraft icon.
    float zoom = 1.0; ///< Map zoom level (adjustable with Z/X).

public:
    explicit MinimapView(const MinimapConfig &c)
        : conf(c),
          tex(LoadTexture(c.mapTexture.c_str())) {
    }

    void update(entt::registry &registry, const float dt) override {
        const auto entity = registry.ctx().get<PlayerEntity>().id;
        const auto player = registry.get<Player>(entity);

        if (IsKeyPressed(KEY_Z)) zoom += 0.4f;
        if (IsKeyPressed(KEY_X)) zoom -= 0.4f;
        if (zoom > 5.0f) zoom = 5.0f;
        if (zoom < 0.2f) zoom = 0.2f;

        const auto aircraftPosition = player.pos - player.offset;
        const auto mapX = aircraftPosition.x / conf.mapsRatio;
        const auto mapZ = aircraftPosition.z / conf.mapsRatio;
        const auto halfSize = static_cast<float>(conf.size) / 2.0f;

        mapCamera.target = (Vector2){mapX, mapZ};
        mapCamera.offset = (Vector2){halfSize, halfSize}; // half widget size, from conf
        mapCamera.rotation = 0.0f;
        mapCamera.zoom = zoom;

        heading = 180.0f - atan2f(player.forward.x, player.forward.z) * RAD2DEG;
    }

    void draw(entt::registry &registry) override {
        const auto view = registry.view<Position2D, MinimapWidget>();
        for (auto [entity, pos]: view.each()) {
            const auto x = static_cast<int>(pos.pos.x);
            const auto y = static_cast<int>(pos.pos.y);
            const int size = conf.size;
            const float halfSize = static_cast<float>(conf.size) / 2.0f;

            mapCamera.offset = (Vector2){pos.pos.x + halfSize, pos.pos.y + halfSize};

            BeginScissorMode(x, y, size, size);
            DrawRectangle(x, y, size, size, GRAY);
            BeginMode2D(mapCamera);
            DrawTexture(tex, 0, 0, WHITE);

            // draw small triangle
            // we divide by zoom to keep it the same size
            const Vector2 planeMapPos = mapCamera.target;
            const float width = 5.0f / zoom;
            const float height = 12.0f / zoom;
            Vector2 v1 = {0.0f, -height / 2.0f};
            Vector2 v2 = {-width / 2.0f, height / 2.0f};
            Vector2 v3 = {width / 2.0f, height / 2.0f};

            v1 = Vector2Add(Vector2Rotate(v1, heading * DEG2RAD), planeMapPos);
            v2 = Vector2Add(Vector2Rotate(v2, heading * DEG2RAD), planeMapPos);
            v3 = Vector2Add(Vector2Rotate(v3, heading * DEG2RAD), planeMapPos);
            DrawTriangle(v1, v2, v3, GREEN);
            DrawTriangleLines(v1, v2, v3, BLACK);


            EndMode2D();
            DrawText(
                TextFormat("Z: x%.1f", zoom),
                x + 5,
                y + 140,
                10,
                BLACK
            );
            EndScissorMode();
        }
    }
};
