#include "Obstacle.h"

Obstacle::Obstacle()
{
    rect = {900, 0, WIDTH, HEIGHT};

    speed = 6;
    groundY = 0;

    passed = false;
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
        passed = false;
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

bool Obstacle::hasPassedPlayer(int playerX)
{
    if (!passed && rect.x + WIDTH < playerX)
    {
        passed = true;
        return true;
    }

    return false;
}

void Obstacle::resetPassed()
{
    passed = false;
}