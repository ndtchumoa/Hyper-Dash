#pragma once

#include "graphics/RenderLayer.h"

#include <SDL2/SDL.h>

class SpriteSheet;

struct SpriteLayer
{
    const SpriteSheet* sheet = nullptr;

    std::size_t frameIndex = 0;

    SDL_Point offset{0,0};

    bool visible = true;

    RenderLayer layer =
        RenderLayer::Body;
};