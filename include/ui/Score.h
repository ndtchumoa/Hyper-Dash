#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class Score
{
public:
    bool init(SDL_Renderer* renderer);

    void addPoint();

    void reset();

    void render(SDL_Renderer* renderer);

    void clean();

private:
    void updateTexture(SDL_Renderer* renderer);

private:
    int value = 0;

    TTF_Font* font = nullptr;

    SDL_Texture* textTexture = nullptr;

    SDL_Rect textRect{};
};