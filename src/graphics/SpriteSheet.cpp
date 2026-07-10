#include "graphics/SpriteSheet.h"

bool SpriteSheet::create(
    SDL_Texture* tex,
    int fw,
    int fh)
{
    clear();

    if (!tex)
        return false;

    m_texture     = tex;
    m_frameWidth  = fw;
    m_frameHeight = fh;

    SDL_QueryTexture(
        m_texture,
        nullptr, nullptr,
        &m_textureWidth,
        &m_textureHeight);

    generateGridFrames();

    return !m_frames.empty();
}

bool SpriteSheet::create(
    SDL_Texture* tex,
    const std::vector<SDL_Rect>& customFrames)
{
    clear();

    if (!tex)
        return false;

    m_texture = tex;

    SDL_QueryTexture(
        m_texture,
        nullptr, nullptr,
        &m_textureWidth,
        &m_textureHeight);

    m_frames = customFrames;

    return !m_frames.empty();
}

void SpriteSheet::generateGridFrames()
{
    m_frames.clear();

    if (m_frameWidth <= 0 || m_frameHeight <= 0)
        return;

    const int cols = m_textureWidth  / m_frameWidth;
    const int rows = m_textureHeight / m_frameHeight;

    m_frames.reserve(
        static_cast<std::size_t>(cols * rows));

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            m_frames.push_back({
                x * m_frameWidth,
                y * m_frameHeight,
                m_frameWidth,
                m_frameHeight
            });
        }
    }
}

SDL_Texture* SpriteSheet::getTexture() const
{
    return m_texture;
}

const SDL_Rect& SpriteSheet::getFrame(
    std::size_t index) const
{
    return m_frames.at(index);
}

std::size_t SpriteSheet::getFrameCount() const
{
    return m_frames.size();
}

int SpriteSheet::getFrameWidth() const
{
    return m_frameWidth;
}

int SpriteSheet::getFrameHeight() const
{
    return m_frameHeight;
}

bool SpriteSheet::empty() const
{
    return m_texture == nullptr || m_frames.empty();
}

void SpriteSheet::clear()
{
    m_texture       = nullptr;
    m_textureWidth  = 0;
    m_textureHeight = 0;
    m_frameWidth    = 0;
    m_frameHeight   = 0;
    m_frames.clear();
}