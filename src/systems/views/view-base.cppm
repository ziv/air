module;
#include <entt/entt.hpp>

export module Views:Base;

import Components;

export class ViewBase {
public:
    virtual ~ViewBase() = default;

    virtual void update(entt::registry &registry, const float dt) {
        // default: no per-frame behavior
    }

    virtual void draw(entt::registry &registry) {
        // default: no per-frame behavior
    }
};
