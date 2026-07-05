#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

struct AnimationFrame
{
    // Vị trí frame trong spritesheet
    SDL_Rect sourceRect{};

    // Offset khi render
    SDL_Point offset{0, 0};

    // Pivot (dùng cho xoay hoặc scale sau này)
    SDL_Point pivot{0, 0};

    // Thời gian hiển thị frame (ms)
    std::uint32_t duration = 100;

    // Cho phép flip riêng từng frame nếu cần
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};