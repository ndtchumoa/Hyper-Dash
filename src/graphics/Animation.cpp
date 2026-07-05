#include "graphics/Animation.h"

void Animation::setFrameSize(
    int width,
    int height)
{
    frameWidth = width;
    frameHeight = height;
}

void Animation::setFrameCount(int count)
{
    frameCount = count;
}

void Animation::setFrameDuration(int duration)
{
    frameDuration = duration;
}

void Animation::update()
{
    counter++;

    if (counter >= frameDuration)
    {
        counter = 0;

        currentFrame++;

        if (currentFrame >= frameCount)
        {
            currentFrame = 0;
        }
    }
}

SDL_Rect Animation::getSourceRect() const
{
    return
    {
        currentFrame * frameWidth,
        0,
        frameWidth,
        frameHeight
    };
}

void Animation::reset()
{
    currentFrame = 0;
    counter = 0;
}