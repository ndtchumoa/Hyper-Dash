#include "TextureManager.h"

#include <iostream>

SDL_Texture* TextureManager::LoadTexture(
    const std::string& path,
    SDL_Renderer* renderer)
{
    SDL_Texture* texture = IMG_LoadTexture(
        renderer,
        path.c_str());

    if (!texture)
    {
        std::cerr
            << "Failed to load texture: "
            << path
            << '\n';

        std::cerr
            << IMG_GetError()
            << '\n';
    }

    return texture;
}

void TextureManager::DestroyTexture(SDL_Texture* texture)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}