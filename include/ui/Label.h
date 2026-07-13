#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

// Label render một dòng text tại vị trí cho trước.
// Không own TTF_Font — lấy từ ResourceManager qua caller.
// Own SDL_Texture* được tạo từ text.

class Label
{
public:

    Label() = default;
    ~Label();

    // font không được own bởi Label.
    bool init(
        SDL_Renderer*    renderer,
        TTF_Font*        font,
        const std::string& text,
        SDL_Color        color = { 255, 255, 255, 255 });

    void setText(
        SDL_Renderer*      renderer,
        const std::string& text);

    void setPosition(int x, int y);

    void setColor(
        SDL_Renderer*    renderer,
        SDL_Color        color);

    void render(SDL_Renderer* renderer) const;

    void clean();

    SDL_Rect getBounds() const;

private:

    void rebuildTexture(SDL_Renderer* renderer);

private:

    TTF_Font*    m_font        = nullptr;   // không own
    SDL_Texture* m_texture     = nullptr;   // own
    SDL_Rect     m_rect{};

    std::string m_text;
    SDL_Color   m_color = { 255, 255, 255, 255 };
};