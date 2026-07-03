#pragma once

#include <SDL2/SDL.h>

class Obstacle
{
public:
    Obstacle();

    void update();
    void render(SDL_Renderer* renderer);

    void setGroundY(int y);

    const SDL_Rect& getRect() const;

private:
    SDL_Rect rect;

    int speed;
    int groundY;

    bool passed;

public:
    static constexpr int WIDTH = 40;
    static constexpr int HEIGHT = 60;

public:
    bool hasPassedPlayer(int playerX);
    void resetPassed();
};