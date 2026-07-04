#include "ui/Score.h"

#include <string>

bool Score::init(SDL_Renderer* renderer)
{
    font = TTF_OpenFont(
        "assets/fonts/PixelOperator.ttf",
        24
    );

    if (!font)
        return false;

    updateTexture(renderer);

    return true;
}

void Score::addPoint()
{
    value++;
}

void Score::reset()
{
    value = 0;
}

void Score::updateTexture(SDL_Renderer* renderer)
{
    if (textTexture)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    SDL_Color color =
    {
        255,
        255,
        255,
        255
    };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            ("Score: " + std::to_string(value)).c_str(),
            color
        );

    if (!surface)
        return;

    textTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    textRect =
    {
        20,
        20,
        surface->w,
        surface->h
    };

    SDL_FreeSurface(surface);
}

void Score::render(SDL_Renderer* renderer)
{
    updateTexture(renderer);

    SDL_RenderCopy(
        renderer,
        textTexture,
        nullptr,
        &textRect
    );
}

void Score::clean()
{
    if (textTexture)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
}