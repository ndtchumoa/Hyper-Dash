#pragma once

#include <SDL2/SDL.h>

class Player
{
public:
    Player();

    void update();
    void render(SDL_Renderer* renderer);

    void jump();
    void setGroundY(int y);

    const SDL_Rect& getRect() const;

    static constexpr int WIDTH = 50;
    static constexpr int HEIGHT = 50;

private:
    SDL_Rect rect;

    float velocityY;
    bool isJumping;
    int groundY;

    static constexpr float gravity = 0.6f;
    static constexpr float jumpForce = -12.0f;
};