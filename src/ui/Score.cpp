#include "ui/Score.h"

#include "resources/ResourceManager.h"

#include <string>
#include <iostream>

bool Score::init(
    SDL_Renderer*    renderer,
    ResourceManager& resources)
{
    // Font thuộc ResourceManager — Score không TTF_OpenFont, không TTF_CloseFont.
    m_font = resources.getFont(FontID::UIRegular);

    if (!m_font)
    {
        std::cerr << "[Score] Failed to get font UIRegular.\n";
        return false;
    }

    m_dirty = true;
    rebuildTexture(renderer);

    return m_textTexture != nullptr;
}

void Score::addPoint()
{
    ++m_value;
    m_dirty = true;
}

void Score::reset()
{
    m_value = 0;
    m_dirty = true;
}

void Score::render(SDL_Renderer* renderer)
{
    if (m_dirty)
        rebuildTexture(renderer);

    if (!m_textTexture)
        return;

    SDL_RenderCopy(renderer, m_textTexture, nullptr, &m_textRect);
}

void Score::clean()
{
    // m_font không destroy — miễn ResourceManager.
    if (m_textTexture)
    {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }
}

int Score::getValue() const
{
    return m_value;
}

void Score::rebuildTexture(SDL_Renderer* renderer)
{
    if (m_textTexture)
    {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }

    if (!m_font || !renderer)
        return;

    const std::string text = "Score: " + std::to_string(m_value);

    SDL_Surface* surface =
        TTF_RenderText_Blended(m_font, text.c_str(), kColor);

    if (!surface)
    {
        std::cerr
            << "[Score] TTF_RenderText_Blended failed: "
            << TTF_GetError() << '\n';
        return;
    }

    m_textTexture =
        SDL_CreateTextureFromSurface(renderer, surface);

    m_textRect = { 20, 20, surface->w, surface->h };

    SDL_FreeSurface(surface);

    m_dirty = false;
}