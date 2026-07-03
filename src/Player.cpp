#include "Player.h"

Player::Player()
    : rect{100, 500, WIDTH, HEIGHT},
      velocityY(0.0f),
      isJumping(false),
      groundY(0)
{
}

void Player::jump()
{
    if (!isJumping)
    {
        velocityY = jumpForce;
        isJumping = true;
    }
}

void Player::update()
{
    velocityY += gravity;

    rect.y += static_cast<int>(velocityY);

    if (rect.y >= groundY)
    {
        rect.y = groundY;
        velocityY = 0.0f;
        isJumping = false;
    }
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 50, 220, 50, 255);
    SDL_RenderCopy(renderer,
               texture,
               nullptr,
               &rect);
}

void Player::setGroundY(int y)
{
    groundY = y;
}

const SDL_Rect& Player::getRect() const
{
    return rect;
}