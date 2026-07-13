#pragma once

#include "ui/Label.h"

#include <SDL2/SDL.h>

// Button = Label + hitbox + trạng thái hover/click.
// Scene tự check isClicked() hoặc isHovered() mỗi frame
// rồi quyết định action — không dùng callback để tránh coupling.
//
// Padding xung quanh text để hitbox rộng hơn label.

class Button
{
public:

    Button() = default;

    bool init(
        SDL_Renderer*      renderer,
        TTF_Font*          font,
        const std::string& text,
        int                x,
        int                y,
        int                paddingX = 20,
        int                paddingY = 10);

    // Cập nhật trạng thái hover/click từ SDL_Event.
    // Gọi mỗi frame trong handleEvents().
    void handleEvent(const SDL_Event& event);

    void render(SDL_Renderer* renderer) const;

    void clean();

    bool isClicked()  const;
    bool isHovered()  const;

    // Đặt lại trạng thái click sau khi đã xử lý.
    void resetClick();

    SDL_Rect getBounds() const;

private:

    void updateHitbox();

private:

    Label    m_label;
    SDL_Rect m_hitbox{};

    int m_paddingX = 20;
    int m_paddingY = 10;

    bool m_hovered = false;
    bool m_clicked = false;

    // Màu sắc theo trạng thái
    static constexpr SDL_Color kColorNormal  = { 255, 255, 255, 255 };
    static constexpr SDL_Color kColorHovered = { 255, 220,  50, 255 };
};