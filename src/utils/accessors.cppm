module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Accessors;

import Components;

constexpr auto PLAYER_ENTITY_CTX = entt::hashed_string("PlayerEntityID");

export void set_player(entt::registry &registry, entt::entity player_entity) {
    // registry.ctx().emplace<PlayerEntity>(player_entity, PLAYER_ENTITY_CTX);
    registry.ctx().emplace_as<entt::entity>(PLAYER_ENTITY_CTX, player_entity);
}

export Player &get_player(entt::registry &registry) {
    // const auto player_entity = registry.ctx().get<PlayerEntity>().id;
    const auto player_entity = registry.ctx().get<entt::entity>(PLAYER_ENTITY_CTX);
    return registry.get<Player>(player_entity);
}

export Vector3 get_offset(entt::registry &registry) {
    return registry.ctx().get<Offset>().offset;
}
