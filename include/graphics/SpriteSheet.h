#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <vector>

class SpriteSheet
{
public:

    SpriteSheet() = default;

    //------------------------------------

    bool create(
        SDL_Texture* texture,
        int frameWidth,
        int frameHeight);

    //------------------------------------

    bool create(
        SDL_Texture* texture,
        const std::vector<SDL_Rect>& frames);

    //------------------------------------

    SDL_Texture* getTexture() const;

    //------------------------------------

    const SDL_Rect&
    getFrame(std::size_t index) const;

    //------------------------------------

    std::size_t
    getFrameCount() const;

    //------------------------------------

    int getFrameWidth() const;

    int getFrameHeight() const;

    //------------------------------------

    bool empty() const;

    //------------------------------------

    void clear();

private:

    void generateGridFrames();

private:

    SDL_Texture* texture = nullptr;

    int textureWidth = 0;
    int textureHeight = 0;

    int frameWidth = 0;
    int frameHeight = 0;

    std::vector<SDL_Rect> frames;
};