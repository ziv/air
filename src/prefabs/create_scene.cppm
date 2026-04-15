module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module ScenePrefabs;

import JsonConfig;
import WorldComponents;
import AircraftComponents;
import RenderComponents;
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

    const Image heightImage = LoadImage(heightmapPath.data());
    const Mesh mesh = GenMeshHeightmap(heightImage, size);
    UnloadImage(heightImage);

    const Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return model;
}


export namespace Factories {
    entt::entity createScene(entt::registry &registry,
                             const JsonConfig &config) {
        const auto conf = config.get<SceneConfig>("/scene");

        const auto entity = registry.create();

        registry.emplace<World>(entity);
        registry.emplace<Position3D>(entity, Vector3Zero(), Vector3Zero());


        // get the cache from registry
        auto &assets = registry.ctx().get<ResourceManager>();


        if (const auto heightMapId = entt::hashed_string(conf.mapHeightmap.data()); !assets.images.contains(heightMapId)) {
            const auto heightmap = LoadImage(conf.mapHeightmap.c_str());
            assets.images.load(heightMapId, heightmap);

            // connect the image to the component
            registry.emplace<Imaged>(entity, assets.images[heightMapId]);
        }

        if (const auto worldModelId = entt::hashed_string(conf.mapTexture.data()); !assets.models.contains(worldModelId)) {
            const auto worldModel = loadWorldModel(
                conf.mapTexture,
                conf.mapHeightmap,
                conf.mapSize
            );
            assets.models.load(worldModelId, worldModel);

            // connect the model to the component
            registry.emplace<Modeled>(entity, assets.models[worldModelId]);
        }


        // scene height map
        // registry.emplace<Imaged>(entity, height);

        // world model
        // registry.emplace<Modeled>(entity, model);

        return entity;
    }
}
