#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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

    // Load nhạc nền (streaming) từ path. Yêu cầu Mix_OpenAudio đã
    // được gọi trước đó (thực hiện trong Game::init()).
    // Trả về nullptr nếu thất bại.
    Mix_Music* loadMusic(std::string_view path);

    // Load sound effect (decode toàn bộ vào memory) từ path. Yêu cầu
    // Mix_OpenAudio đã được gọi trước đó.
    // Trả về nullptr nếu thất bại.
    Mix_Chunk* loadSfx(std::string_view path);
}