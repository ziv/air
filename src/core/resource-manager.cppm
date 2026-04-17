module;
#include <memory>
#include <raylib.h>
#include <string>
#include <entt/entt.hpp>

// todo should be template

export module ResourceManager;

export template<typename T, T (*Loader)(const char *), void (*Unloader)(T)>
struct ResourceLoader {
    T res;

    explicit ResourceLoader(const std::string &path) : res(Loader(path.c_str())) {
    }

    explicit ResourceLoader(T ready) : res(ready) {
    }

    ~ResourceLoader() {
        Unloader(res);
    }

    ResourceLoader(const ResourceLoader &) = delete;

    ResourceLoader &operator=(const ResourceLoader &) = delete;
};

// its loader is different from the template ones
export struct FragmentShaderLoader {
    Shader res;

    explicit FragmentShaderLoader(const std::string &path) : res(LoadShader(nullptr, path.c_str())) {
    }

    explicit FragmentShaderLoader(const Shader ready) : res(ready) {
    }

    ~FragmentShaderLoader() {
        UnloadShader(res);
    }

    FragmentShaderLoader(const FragmentShaderLoader &) = delete;

    FragmentShaderLoader &operator=(const FragmentShaderLoader &) = delete;
};

export
{
    using TextureResourceLoader = ResourceLoader<Texture2D, LoadTexture, UnloadTexture>;
    using ModelResourceLoader = ResourceLoader<Model, LoadModel, UnloadModel>;
    using ImageResourceLoader = ResourceLoader<Image, LoadImage, UnloadImage>;
}

export struct ResourceManager {
    entt::resource_cache<TextureResourceLoader> tex;
    entt::resource_cache<ModelResourceLoader> mdl;
    entt::resource_cache<ImageResourceLoader> img;

    // fragment shaders only
    entt::resource_cache<FragmentShaderLoader> fs;
};


constexpr auto MANAGER_ID = entt::hashed_string("ResourceManager");

export void create_resource_manager(entt::registry &registry) {
    registry.ctx().emplace_as<ResourceManager>(MANAGER_ID);
}

export ResourceManager &get_resource_manager(entt::registry &registry) {
    return registry.ctx().get<ResourceManager>(MANAGER_ID);
}
