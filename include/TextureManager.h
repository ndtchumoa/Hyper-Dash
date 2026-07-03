#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

class TextureManager
{
public:
    static SDL_Texture* LoadTexture(
        const std::string& path,
        SDL_Renderer* renderer);

    static void DestroyTexture(SDL_Texture* texture);
};