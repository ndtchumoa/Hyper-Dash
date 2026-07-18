#include "resources/ResourceLoader.h"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

namespace ResourceLoader
{
    SDL_Texture* loadTexture(
        SDL_Renderer*    renderer,
        std::string_view path)
    {
        if (!renderer)
        {
            std::cerr << "[ResourceLoader] Renderer is null.\n";
            return nullptr;
        }

        if (path.empty())
        {
            std::cerr << "[ResourceLoader] Empty path for texture.\n";
            return nullptr;
        }

        const std::string pathStr{ path };

        SDL_Texture* texture =
            IMG_LoadTexture(renderer, pathStr.c_str());

        if (!texture)
        {
            std::cerr
                << "[ResourceLoader] Failed to load texture:\n"
                << "  Path : " << pathStr  << '\n'
                << "  Error: " << IMG_GetError() << '\n';
        }

        return texture;
    }

    TTF_Font* loadFont(
        std::string_view path,
        int              size)
    {
        if (path.empty() || size <= 0)
        {
            std::cerr
                << "[ResourceLoader] Invalid font path or size.\n";
            return nullptr;
        }

        const std::string pathStr{ path };

        TTF_Font* font = TTF_OpenFont(pathStr.c_str(), size);

        if (!font)
        {
            std::cerr
                << "[ResourceLoader] Failed to load font:\n"
                << "  Path : " << pathStr    << '\n'
                << "  Size : " << size       << '\n'
                << "  Error: " << TTF_GetError() << '\n';
        }

        return font;
    }

    Mix_Music* loadMusic(std::string_view path)
    {
        if (path.empty())
        {
            std::cerr << "[ResourceLoader] Empty path for music.\n";
            return nullptr;
        }

        const std::string pathStr{ path };

        Mix_Music* music = Mix_LoadMUS(pathStr.c_str());

        if (!music)
        {
            std::cerr
                << "[ResourceLoader] Failed to load music:\n"
                << "  Path : " << pathStr << '\n'
                << "  Error: " << Mix_GetError() << '\n';
        }

        return music;
    }

    Mix_Chunk* loadSfx(std::string_view path)
    {
        if (path.empty())
        {
            std::cerr << "[ResourceLoader] Empty path for sfx.\n";
            return nullptr;
        }

        const std::string pathStr{ path };

        Mix_Chunk* chunk = Mix_LoadWAV(pathStr.c_str());

        if (!chunk)
        {
            std::cerr
                << "[ResourceLoader] Failed to load sfx:\n"
                << "  Path : " << pathStr << '\n'
                << "  Error: " << Mix_GetError() << '\n';
        }

        return chunk;
    }
}