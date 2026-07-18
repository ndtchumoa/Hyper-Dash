#pragma once

#include <SDL2/SDL.h>

class Obstacle
{
public:

    // sourceRect: vùng cắt trong texture (hỗ trợ texture dạng
    // spritesheet chứa nhiều obstacle — xem ObstacleCatalog).
    // renderWidth/renderHeight: kích thước thực tế render lên màn
    // hình VÀ cũng là kích thước hitbox (Obstacle không tự inset).
    //
    // Khác obstacle kind có thể có size khác nhau (không còn 1 hằng
    // số kWidth/kHeight chung cho mọi obstacle như trước) — nguồn sự
    // thật duy nhất cho size nằm ở ObstacleCatalog::resolve(), không
    // lặp lại ở đây.
    Obstacle(
        SDL_Texture* texture,
        SDL_Rect     sourceRect,
        int          renderWidth,
        int          renderHeight,
        int          startX,
        int          groundY,
        float        speedPixelsPerSecond);

    // deltaTime tính bằng giây.
    void update(float deltaTime);

    void render(SDL_Renderer* renderer) const;

    bool isOffScreen()           const;
    bool hasPassedPlayer(int playerX);

    SDL_Rect getBounds() const;

private:

    SDL_Texture* m_texture = nullptr;
    SDL_Rect     m_sourceRect{};
    SDL_Rect     m_rect{};
    float        m_speed  = 0.0f;   // pixels/giây
    float        m_posX   = 0.0f;   // vị trí thực (float để tránh drift)
    bool         m_passed = false;
};
