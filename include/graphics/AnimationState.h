#pragma once

#include <cstdint>

enum class AnimationPlaybackState : std::uint8_t
{
    Stopped = 0,

    Playing,

    Paused,

    Finished
};