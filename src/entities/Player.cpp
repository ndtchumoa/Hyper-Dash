#include "entities/Player.h"

#include "systems/AssetManager.h"

bool Player::init(
    AssetManager& assets,
    int ground)
{
    groundY = ground;

    //---------------------------------
    // Load Layers
    //---------------------------------

    skin =
        assets.getTexture(
        "assets/textures/player/Character skin colors/Male Skin1.png");

    shirt =
        assets.getTexture(
        "assets/textures/player/Male Clothing/Shirt v2.png");

    pants =
        assets.getTexture(
        "assets/textures/player/Male Clothing/Pants.png");

    hair =
        assets.getTexture(
        "assets/textures/player/Male Hair/Male Hair1.png");

    shoes =
        assets.getTexture(
        "assets/textures/player/Male Clothing/Shoes.png");

    //---------------------------------
    // Animation
    //---------------------------------

    animation.setFrameSize(
        FRAME_WIDTH,
        FRAME_HEIGHT);

    animation.setFrameCount(13);

    animation.setFrameDuration(6);

    //---------------------------------
    // Position
    //---------------------------------

    dstRect =
    {
        120,
        groundY - HEIGHT,
        WIDTH,
        HEIGHT
    };

    velocityY = 0.0f;

    onGround = true;

    return true;
}

void Player::update()
{
    animation.update();

    velocityY += GRAVITY;

    dstRect.y += (int)velocityY;

    if (dstRect.y >= groundY - HEIGHT)
    {
        dstRect.y = groundY - HEIGHT;

        velocityY = 0;

        onGround = true;
    }
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_Rect src =
        animation.getSourceRect();

    SDL_RenderCopy(
        renderer,
        skin,
        &src,
        &dstRect);

    SDL_RenderCopy(
        renderer,
        pants,
        &src,
        &dstRect);

    SDL_RenderCopy(
        renderer,
        shirt,
        &src,
        &dstRect);

    SDL_RenderCopy(
        renderer,
        hair,
        &src,
        &dstRect);

    SDL_RenderCopy(
        renderer,
        shoes,
        &src,
        &dstRect);
}

void Player::jump()
{
    if (!onGround)
        return;

    velocityY = JUMP_FORCE;

    onGround = false;
}

void Player::reset()
{
    dstRect.x = 120;

    dstRect.y = groundY - HEIGHT;

    velocityY = 0;

    onGround = true;

    animation.reset();
}

SDL_Rect Player::getBounds() const
{
    return dstRect;
}