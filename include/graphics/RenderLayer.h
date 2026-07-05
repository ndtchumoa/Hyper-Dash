#pragma once

#include <cstdint>

enum class RenderLayer : std::uint8_t
{
    Background = 0,

    Shadow,

    Body,

    Clothing,

    Hair,

    Helmet,

    Weapon,

    Effect,

    UI
};