module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderDebug;

import WorldComponents;

export void RenderDebug(entt::registry &registry) {
    auto view = registry.view<Player, PlayerInputs>();

    if (view.begin() == view.end()) {
        return;
    }

    const entt::entity entity = view.front();
    auto [player, inputs] = view.get<Player, PlayerInputs>(entity);

    // auto [pos, utils, engine, controls, acc, angVel, vel, orient] = view.get<Player, Position3D, AircraftUtils, Engine, AircraftControls, LinerAcceleration, AngularVelocity, LinearVelocity, Orientation>(entity);
    //
    //
    int y = 10;
    const int margin = 15;
    const int fs = 10;
    DrawRectangle(5, 5, 150, 340, BEIGE);
    DrawText(TextFormat("Px: %0.00f", player.pos.x), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Pz: %0.00f", player.pos.z), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Py: %0.00f", player.pos.y), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Brk: %s", inputs.brakes ? "ON" : "OFF"), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Ger: %s", inputs.gear ? "DN" : "UP"), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Spd: %f", player.speed), margin, y, fs, BLACK);


    y += margin;
    DrawText(TextFormat("Ct: %f", inputs.throttle), margin, y, fs, BLACK);
    // y += margin;
    // DrawText(TextFormat("Et: %f", player.throttle), margin, y, fs, BLACK);

    y += margin;
    DrawText(TextFormat("LVx: %f", player.velocity.x), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("LVz: %f", player.velocity.z), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("LVy: %f", player.velocity.y), margin, y, fs, BLACK);
    //
    // y += margin;
    // DrawText(TextFormat("AVx: %f", angVel.velocity.x), margin, y, fs, BLACK);
    // y += margin;
    // DrawText(TextFormat("AVz: %f", angVel.velocity.z), margin, y, fs, BLACK);
    // y += margin;
    // DrawText(TextFormat("AVy: %f", angVel.velocity.y), margin, y, fs, BLACK);
    //
    // y += margin;
    // const float speed = Vector3Length(vel.velocity);
    // DrawText(TextFormat("Spd: %.1f", speed), margin, y, fs, BLACK);
    //
    // y += margin;
    // float heading = atan2f(orient.forward.x, orient.forward.z) * RAD2DEG;
    // if (heading < 0.0f) heading += 360.0f;
    // DrawText(TextFormat("Hdg: %.0f", heading), margin, y, fs, BLACK);
    //
    // y += margin;
    // const float pitch = asinf(-orient.forward.y) * RAD2DEG;
    // DrawText(TextFormat("Pit: %.1f", pitch), margin, y, fs, BLACK);
    //
    // y += margin;
    // const float roll = atan2f(orient.right.y, orient.up.y) * RAD2DEG;
    // DrawText(TextFormat("Rol: %.1f", roll), margin, y, fs, BLACK);
    //
    // y += margin;
    // DrawText(TextFormat("Cq: %.1f", controls.yaw), margin, y, fs, BLACK);

}
