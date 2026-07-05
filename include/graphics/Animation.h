#pragma once

#include <SDL2/SDL.h>

class Animation
{
public:
    Animation() = default;

    void setFrameSize(
        int width,
        int height
    );

    void setFrameCount(int count);

    void setFrameDuration(int duration);

    void update();

    SDL_Rect getSourceRect() const;

    void reset();

private:
    int frameWidth = 0;

    int frameHeight = 0;

    int frameCount = 1;

    int frameDuration = 8;

    int currentFrame = 0;

    int counter = 0;
};