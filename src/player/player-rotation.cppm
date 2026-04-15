module;
#include <entt/entt.hpp>
#include "../lib/ray.hpp"

export module Player:Rotation;

import Components;
import Types;
import Helpers;
import :Config;


export class PlayerRotation {
    PlayerTransformationConfig conf;

public:
    explicit PlayerRotation(const PlayerTransformationConfig &c) : conf(c) {
    }

    void update(entt::registry &registry, const float dt) const {
        const auto entity = registry.ctx().get<PlayerEntity>().id;
        auto [player, inputs] = registry.get<Player, PlayerInputs>(entity);

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
            player.forward = Vector3Normalize(Vector3RotateByQuaternion(WorldForward(), player.rotation));
            player.up = Vector3Normalize(Vector3RotateByQuaternion(WorldUp(), player.rotation));
            player.right = Vector3Normalize(Vector3RotateByQuaternion(WorldRight(), player.rotation));
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
};
