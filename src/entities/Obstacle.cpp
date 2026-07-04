#include "entities/Obstacle.h"

Obstacle::Obstacle(
    SDL_Texture* tex,
    int startX,
    int groundY,
    float moveSpeed)
    : texture(tex),
      speed(moveSpeed)
{
    rect =
    {
        startX,
        groundY - HEIGHT,
        WIDTH,
        HEIGHT
    };
}

void Obstacle::update()
{
    rect.x -= static_cast<int>(speed);
}

void Obstacle::render(SDL_Renderer* renderer)
{
    if (texture)
    {
        SDL_RenderCopy(
            renderer,
            texture,
            nullptr,
            &rect
        );
    }
    else
    {
        SDL_SetRenderDrawColor(
            renderer,
            220,
            60,
            60,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &rect
        );
    }
}

SDL_Rect Obstacle::getBounds() const
{
    return rect;
}

bool Obstacle::hasPassedPlayer(int playerX)
{
    if (!passed && rect.x + rect.w < playerX)
    {
        passed = true;
        return true;
    }

    return false;
}

bool Obstacle::isOffScreen() const
{
    return rect.x + rect.w < 0;
}