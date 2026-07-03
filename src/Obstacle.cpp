#include "Obstacle.h"

Obstacle::Obstacle()
{
    rect = {900, 0, WIDTH, HEIGHT};

    speed = 6;
    groundY = 0;
}

void Obstacle::setGroundY(int y)
{
    groundY = y;
    rect.y = groundY;
}

void Obstacle::update()
{
    rect.x -= speed;

    if (rect.x + WIDTH < 0)
    {
        rect.x = 800;
    }
}

void Obstacle::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 220, 40, 40, 255);
    SDL_RenderFillRect(renderer, &rect);
}

const SDL_Rect& Obstacle::getRect() const
{
    return rect;
}