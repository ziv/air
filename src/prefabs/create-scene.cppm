module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Prefabs:Scene;

import JsonConfig;
import Components;
import RaylibResource;
import Types;
import ResourceManager;

struct CreateWorldRet {
    ModelHandle model;
    ImageHandle heightmap;
};

Model loadWorldModel(const std::string &texturePath, const std::string &heightmapPath, const Vector3 &size) {
    // our world texture
    const Image textureImage = LoadImage(texturePath.c_str());
    const Texture2D texture = LoadTextureFromImage(textureImage);
    UnloadImage(textureImage);

    // our world heightmap
    const Image heightImage = LoadImage(heightmapPath.data());
    const Mesh mesh = GenMeshHeightmap(heightImage, size);
    UnloadImage(heightImage);

    // our world model
    const Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return model;
}


export namespace Factories {
    entt::entity createScene(entt::registry &registry,
                             const JsonConfig &config) {
        const auto conf = config.get<SceneConfig>("/scene");
        auto &assets = get_resource_manager(registry);

        const auto entity = registry.create();

        registry.emplace<World>(entity);
        registry.emplace<Position3D>(entity, Vector3Zero(), Vector3Zero());

        // attach resources from cache and generate world

        if (const auto tex_id = entt::hashed_string(conf.mapTexture.data()); assets.tex.contains(tex_id)) {
            registry.emplace<WithTexture>(entity, assets.tex[tex_id]);
        } else {
            TraceLog(LOG_WARNING, "scene texture '%s' not found in cache", conf.mapTexture.c_str());
        }

        if (const auto hm_id = entt::hashed_string(conf.mapHeightmap.data()); assets.img.contains(hm_id)) {
            registry.emplace<WithImage>(entity, assets.img[hm_id]);
        } else {
            TraceLog(LOG_WARNING, "scene heightmap '%s' not found in cache", conf.mapHeightmap.c_str());
        }

        constexpr auto model_id = entt::hashed_string("world_model");
        if (!assets.mdl.contains(model_id)) {
            const auto model = loadWorldModel(conf.mapTexture, conf.mapHeightmap, conf.mapSize);
            assets.mdl.load(model_id, model);
        }
        registry.emplace<WithModel>(entity, assets.mdl[model_id]);

        return entity;
    }
}
