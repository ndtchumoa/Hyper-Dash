#pragma once

#include <cstdint>

enum class AnimationID : std::uint16_t
{
    None = 0,

    PlayerIdle,
    PlayerWalk,
    PlayerRun,
    PlayerJump,
    PlayerFall,
    PlayerAttack,
    PlayerDeath,

    GoblinIdle,
    GoblinWalk,
    GoblinAttack,
    GoblinDeath
};

#include <functional>

namespace std
{
    template<>
    struct hash<AnimationID>
    {
        std::size_t operator()(
            const AnimationID& id) const noexcept
        {
            return static_cast<std::size_t>(id);
        }
    };
}