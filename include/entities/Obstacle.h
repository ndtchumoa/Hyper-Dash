#pragma once

#include <SDL2/SDL.h>

class Obstacle
{
public:
    Obstacle(
        SDL_Texture* texture,
        int startX,
        int groundY,
        float speed
    );

    void update();

    void render(SDL_Renderer* renderer);

    bool isOffScreen() const;

    bool hasPassedPlayer(int playerX);

    SDL_Rect getBounds() const;

private:
    SDL_Texture* texture = nullptr;

    SDL_Rect rect{};

    float speed = 0.0f;

    bool passed = false;

public:
    static constexpr int WIDTH = 48;
    static constexpr int HEIGHT = 64;
};