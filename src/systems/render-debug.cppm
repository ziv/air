module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module RenderDebug;

import WorldComponents;
import RenderComponents;
import AircraftComponents;

export void RenderDebug(entt::registry &registry) {
    auto view = registry.view<Player, Position3D, AircraftUtils, Engine, AircraftControls, Acceleration, AngularAcceleration, Velocity, Orientation>();

    if (view.begin() == view.end()) {
        return;
    }

    const entt::entity entity = view.front();

    auto [pos, utils, engine, controls, acc, angAcc, vel, orient] = view.get<Player, Position3D, AircraftUtils, Engine, AircraftControls, Acceleration, AngularAcceleration, Velocity, Orientation>(entity);


    int y = 10;
    const int margin = 15;
    const int fs = 10;
    DrawRectangle(5, 5, 150, 340, BEIGE);
    DrawText(TextFormat("Px: %0.00f", pos.pos.x), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Pz: %0.00f", pos.pos.z), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Py: %0.00f", pos.pos.y), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Brk: %s", utils.brake ? "ON" : "OFF"), margin, y, fs, BLACK);

    y += margin;
    DrawText(TextFormat("Gear: %s", utils.gear ? "DN" : "UP"), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Ct: %f", controls.throttle), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("Et: %f", engine.throttle), margin, y, fs, BLACK);

    y += margin;
    DrawText(TextFormat("LAx: %f", acc.linear.x), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("LAz: %f", acc.linear.z), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("LAy: %f", acc.linear.y), margin, y, fs, BLACK);

    y += margin;
    DrawText(TextFormat("AAx: %f", angAcc.angular.x), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("AAz: %f", angAcc.angular.z), margin, y, fs, BLACK);
    y += margin;
    DrawText(TextFormat("AAy: %f", angAcc.angular.y), margin, y, fs, BLACK);

    y += margin;
    const float speed = Vector3Length(vel.velocity);
    DrawText(TextFormat("Spd: %.1f", speed), margin, y, fs, BLACK);

    y += margin;
    float heading = atan2f(orient.forward.x, orient.forward.z) * RAD2DEG;
    if (heading < 0.0f) heading += 360.0f;
    DrawText(TextFormat("Hdg: %.0f", heading), margin, y, fs, BLACK);

    y += margin;
    const float pitch = asinf(-orient.forward.y) * RAD2DEG;
    DrawText(TextFormat("Pit: %.1f", pitch), margin, y, fs, BLACK);

    y += margin;
    const float roll = atan2f(orient.right.y, orient.up.y) * RAD2DEG;
    DrawText(TextFormat("Rol: %.1f", roll), margin, y, fs, BLACK);

}
