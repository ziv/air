module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Player:Utils;

import Components;
import Types;
import Helpers;
import :Config;

export Player getPlayer(entt::registry &registry) {
    return registry.get<Player>(registry.ctx().get<PlayerEntity>().id);
}
