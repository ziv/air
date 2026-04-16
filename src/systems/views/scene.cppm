module;
#include "../../lib/ray.hpp"
#include <entt/entt.hpp>

export module Views:Scene;

import Types;
import Helpers;
import Components;
import RaylibResource;
import :Base;

Model loadWorldModel(const std::string &texturePath, const std::string &heightmapPath, const Vector3 &size) {
    // our world texture
    const Image textureImage = LoadImage(texturePath.c_str());
    const Texture2D texture = LoadTextureFromImage(textureImage);
    UnloadImage(textureImage);

    const Image heightImage = LoadImage(heightmapPath.data());
    const Mesh mesh = GenMeshHeightmap(heightImage, size);
    UnloadImage(heightImage);

    const Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return model;
}

export class SceneView : public ViewBase {
    ModelHandle map;
    ImageHandle heightmap;

public:
    explicit SceneView(const SceneConfig &c)
        : map(loadWorldModel(c.mapTexture, c.mapTexture, c.mapSize)),
          heightmap(LoadImage(c.mapHeightmap.c_str())) {
    }

    void draw(entt::registry &registry) override {
        const auto offset = registry.ctx().get<Offset>().offset;
        DrawModel(map, offset, 1.0f, WHITE);
    }
};
