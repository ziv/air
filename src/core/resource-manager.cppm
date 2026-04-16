module;
#include <memory>
#include <raylib.h>
#include <string>
#include <entt/entt.hpp>


export module ResourceManager;

export struct ModelResource {
    Model model{};

    ModelResource() = default;

    explicit ModelResource(Model m) : model(m) {
    }

    ~ModelResource() {
        if (model.meshes != nullptr) {
            UnloadModel(model);
        }
    }

    ModelResource(ModelResource &&other) noexcept : model(other.model) {
        other.model = {};
    }

    ModelResource &operator=(ModelResource &&other) noexcept {
        if (this != &other) {
            if (model.meshes != nullptr) UnloadModel(model);
            model = other.model;
            other.model = {};
        }
        return *this;
    }

    ModelResource(const ModelResource &) = delete;

    ModelResource &operator=(const ModelResource &) = delete;
};

struct ResourceLoader {
    using result_type = std::shared_ptr<ModelResource>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<ModelResource>(LoadModel(path.c_str()));
    }
};

export struct ImageResource {
    Image image{};

    ImageResource() = default;

    explicit ImageResource(Image img) : image(img) {
    }

    ~ImageResource() {
        if (image.data != nullptr) {
            UnloadImage(image);
        }
    }

    ImageResource(ImageResource &&other) noexcept : image(other.image) {
        other.image = {};
    }

    ImageResource &operator=(ImageResource &&other) noexcept {
        if (this != &other) {
            if (image.data != nullptr) UnloadImage(image);
            image = other.image;
            other.image = {};
        }
        return *this;
    }

    ImageResource(const ImageResource &) = delete;

    ImageResource &operator=(const ImageResource &) = delete;
};

struct ImageLoader {
    using result_type = std::shared_ptr<ImageResource>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<ImageResource>(LoadImage(path.c_str()));
    }
};

export struct ResourceManager {
    entt::resource_cache<ModelResource> models;
    entt::resource_cache<ImageResource> images;

    // entt::resource_cache<TextureResource, TextureLoader> textures;
    // entt::resource_cache<ShaderResource, ShaderLoader> shaders;
};
