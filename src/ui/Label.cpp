#include "ui/Label.h"

#include <iostream>

Label::~Label()
{
    clean();
}

bool Label::init(
    SDL_Renderer*      renderer,
    TTF_Font*          font,
    const std::string& text,
    SDL_Color          color)
{
    m_font  = font;
    m_text  = text;
    m_color = color;

    rebuildTexture(renderer);

    return m_texture != nullptr;
}

void Label::setText(
    SDL_Renderer*      renderer,
    const std::string& text)
{
    if (m_text == text)
        return;

    m_text = text;
    rebuildTexture(renderer);
}

void Label::setPosition(int x, int y)
{
    m_rect.x = x;
    m_rect.y = y;
}

void Label::setColor(
    SDL_Renderer* renderer,
    SDL_Color     color)
{
    m_color = color;
    rebuildTexture(renderer);
}

void Label::render(SDL_Renderer* renderer) const
{
    if (!m_texture)
        return;

    SDL_RenderCopy(renderer, m_texture, nullptr, &m_rect);
}

void Label::clean()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    m_font = nullptr;
}

SDL_Rect Label::getBounds() const
{
    return m_rect;
}

void Label::rebuildTexture(SDL_Renderer* renderer)
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    if (!m_font || !renderer || m_text.empty())
        return;

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            m_font,
            m_text.c_str(),
            m_color);

    if (!surface)
    {
        std::cerr
            << "[Label] TTF_RenderText_Blended failed: "
            << TTF_GetError() << '\n';
        return;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Giữ x, y hiện tại — chỉ update w, h từ surface mới.
    m_rect.w = surface->w;
    m_rect.h = surface->h;

    SDL_FreeSurface(surface);
}