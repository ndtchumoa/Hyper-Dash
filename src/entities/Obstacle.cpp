#include "entities/Obstacle.h"

Obstacle::Obstacle(
    SDL_Texture* texture,
    int          startX,
    int          groundY,
    float        speedPixelsPerSecond)
    : m_texture(texture)
    , m_speed(speedPixelsPerSecond)
    , m_posX(static_cast<float>(startX))
{
    m_rect =
    {
        startX,
        groundY - kHeight,
        kWidth,
        kHeight
    };
}

void Obstacle::update(float deltaTime)
{
    // Dùng float position để tránh pixel drift khi tốc độ không nguyên.
    m_posX  -= m_speed * deltaTime;
    m_rect.x = static_cast<int>(m_posX);
}

void Obstacle::render(SDL_Renderer* renderer) const
{
    if (m_texture)
    {
        SDL_RenderCopy(renderer, m_texture, nullptr, &m_rect);
    }
    else
    {
        // Fallback placeholder khi texture chưa load được.
        SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);
        SDL_RenderFillRect(renderer, &m_rect);
    }
}

SDL_Rect Obstacle::getBounds() const
{
    return m_rect;
}

bool Obstacle::hasPassedPlayer(int playerX)
{
    if (!m_passed && m_rect.x + m_rect.w < playerX)
    {
        m_passed = true;
        return true;
    }

    return false;
}

bool Obstacle::isOffScreen() const
{
    return m_rect.x + m_rect.w < 0;
}