#pragma once

#include <SDL2/SDL.h>

class Obstacle
{
public:

    Obstacle(
        SDL_Texture* texture,
        int          startX,
        int          groundY,
        float        speedPixelsPerSecond);

    // deltaTime tính bằng giây.
    void update(float deltaTime);

    void render(SDL_Renderer* renderer) const;

    bool isOffScreen()           const;
    bool hasPassedPlayer(int playerX);

    SDL_Rect getBounds() const;

    static constexpr int kWidth  = 48;
    static constexpr int kHeight = 64;

private:

    SDL_Texture* m_texture = nullptr;
    SDL_Rect     m_rect{};
    float        m_speed  = 0.0f;   // pixels/giây
    float        m_posX   = 0.0f;   // vị trí thực (float để tránh drift)
    bool         m_passed = false;
};