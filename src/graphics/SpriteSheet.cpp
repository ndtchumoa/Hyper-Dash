#include "graphics/SpriteSheet.h"

bool SpriteSheet::create(
    SDL_Texture* tex,
    int fw,
    int fh)
{
    clear();

    if (!tex)
        return false;

    texture = tex;

    frameWidth = fw;
    frameHeight = fh;

    SDL_QueryTexture(
        texture,
        nullptr,
        nullptr,
        &textureWidth,
        &textureHeight);

    generateGridFrames();

    return true;
}

bool SpriteSheet::create(
    SDL_Texture* tex,
    const std::vector<SDL_Rect>& customFrames)
{
    clear();

    if (!tex)
        return false;

    texture = tex;

    SDL_QueryTexture(
        texture,
        nullptr,
        nullptr,
        &textureWidth,
        &textureHeight);

    frames = customFrames;

    return true;
}

void SpriteSheet::generateGridFrames()
{
    frames.clear();

    if (frameWidth <= 0 || frameHeight <= 0)
        return;

    int cols = textureWidth / frameWidth;
    int rows = textureHeight / frameHeight;

    frames.reserve(cols * rows);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            SDL_Rect r;

            r.x = x * frameWidth;
            r.y = y * frameHeight;
            r.w = frameWidth;
            r.h = frameHeight;

            frames.push_back(r);
        }
    }
}

SDL_Texture*
SpriteSheet::getTexture() const
{
    return texture;
}

const SDL_Rect&
SpriteSheet::getFrame(
    std::size_t index) const
{
    return frames.at(index);
}

std::size_t
SpriteSheet::getFrameCount() const
{
    return frames.size();
}

int
SpriteSheet::getFrameWidth() const
{
    return frameWidth;
}

int
SpriteSheet::getFrameHeight() const
{
    return frameHeight;
}

bool
SpriteSheet::empty() const
{
    return texture == nullptr
        || frames.empty();
}

void
SpriteSheet::clear()
{
    texture = nullptr;

    textureWidth = 0;
    textureHeight = 0;

    frameWidth = 0;
    frameHeight = 0;

    frames.clear();
}