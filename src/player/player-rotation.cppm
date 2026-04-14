module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module PlayerRotation;

import WorldComponents;
import Types;
import Helpers;

struct PlayerTransformationConfig {
    MeterPerSecond maxSpeed = 600.0f; ///< Reference speed for normalising control authority.
    // MeterPerSecond vleSpeed = 150.0f; ///< Max gear-extended speed; above this → turbulence.
    // MeterPerSecond stallSpeed = 65.0f; ///< Below this speed aerodynamic effects diminish.
    Ratio bankInduceYawRatio = 0.2f; ///< Adverse yaw factor from bank angle.
    Ratio liftLossPitchRatio = 0.1f; ///< Nose-down pitch tendency when lift vector tilts.
};

export class PlayerRotation {
    PlayerTransformationConfig conf;

public:
    void update(entt::registry &registry, const float dt) const {
        for (const auto view = registry.view<Player, PlayerInputs>(); auto [entity, player, inputs]: view.each()) {
            const auto speedRatio = player.speed / conf.maxSpeed;

            // flying mode rotation
            if (player.pos.y > 10.0f) {
                // Adverse yaw: bank angle induces a yaw moment toward the lowered wing.
                // https://en.wikipedia.org/wiki/Adverse_yaw
                const auto bankInducedYaw = player.speed < 0.001f ? 0.0f : player.right.y * conf.bankInduceYawRatio * dt;

                // Lift-loss pitch-down: when the lift vector is no longer purely vertical the
                // aircraft tends to pitch nose-down.
                // https://en.wikipedia.org/wiki/Stall_(fluid_dynamics)
                const auto liftLossPitch = player.speed < 0.001f ? 0.0f : (1.0f - player.up.y) * conf.liftLossPitchRatio * dt;

                // more speed equals more steering except yaw
                const auto totalPitch = (inputs.pitch + liftLossPitch) * speedRatio;
                const auto totalYaw = inputs.yaw + bankInducedYaw;
                const auto totalRoll = inputs.roll * speedRatio;

                // apply the changes
                const auto qPitch = QuaternionFromAxisAngle(player.right, totalPitch);
                const auto qYaw = QuaternionFromAxisAngle(player.up, totalYaw);
                const auto qRoll = QuaternionFromAxisAngle(player.forward, totalRoll);

                const auto qDelta = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

                player.rotation = QuaternionNormalize(QuaternionMultiply(qDelta, player.rotation));
                player.forward = Vector3Normalize(Vector3RotateByQuaternion((Vector3){0.0f, 0.0f, 1.0f}, player.rotation));
                player.up = Vector3Normalize(Vector3RotateByQuaternion((Vector3){0.0f, 1.0f, 0.0f}, player.rotation));
                player.right = Vector3Normalize(Vector3RotateByQuaternion((Vector3){-1.0f, 0.0f, 0.0f}, player.rotation));
            }

            // ground mode
            else {
                // apply the changes (more speed equals more steering except yaw)
                // on ground pitch can be positive only
                // todo this is a bug, I can do negative pitch (lower my nose), but I can not lower it more than the ground...
                const auto pitch = inputs.pitch > 0.0f ? inputs.pitch : 0.0f;
                const auto qPitch = QuaternionFromAxisAngle(player.right, pitch * speedRatio);
                const auto qYaw = QuaternionFromAxisAngle(player.up, inputs.yaw);
                const auto qRoll = QuaternionFromAxisAngle(player.forward, 0);

                // update the quaternion and normalize, then recalculate vectors
                const auto qDelta = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

                player.rotation = QuaternionNormalize(QuaternionMultiply(qDelta, player.rotation));
                player.forward = Vector3Normalize(Vector3RotateByQuaternion(WorldForward(), player.rotation));
                player.up = Vector3Normalize(Vector3RotateByQuaternion(WorldUp(), player.rotation));
                player.right = Vector3Normalize(Vector3RotateByQuaternion(WorldRight(), player.rotation));
            }
        }
    }
};
