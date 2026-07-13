#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string_view>

namespace ResourceLoader
{
    // Load texture từ path.
    // Trả về nullptr nếu thất bại.
    SDL_Texture* loadTexture(
        SDL_Renderer*    renderer,
        std::string_view path);

    // Load font từ path với size cho trước.
    // Trả về nullptr nếu thất bại.
    TTF_Font* loadFont(
        std::string_view path,
        int              size);
}