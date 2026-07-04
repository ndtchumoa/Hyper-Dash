#pragma once

#include <SDL2/SDL.h>

#include "systems/Animation.h"

class AssetManager;

enum class PlayerState
{
    Idle,
    Run,
    Jump
};

class Player
{
public:
    bool init(
        AssetManager& assets,
        int groundY);

    void update();

    void render(SDL_Renderer* renderer);

    void jump();

    void reset();

    SDL_Rect getBounds() const;

private:

    //==========================
    // Character Layers
    //==========================

    SDL_Texture* skin = nullptr;
    SDL_Texture* shirt = nullptr;
    SDL_Texture* pants = nullptr;
    SDL_Texture* hair = nullptr;
    SDL_Texture* shoes = nullptr;

    //==========================
    // Animation
    //==========================

    Animation animation;

    PlayerState state = PlayerState::Run;

    //==========================
    // Physics
    //==========================

    SDL_Rect dstRect{};

    float velocityY = 0.0f;

    int groundY = 0;

    bool onGround = true;

    static constexpr float GRAVITY = 0.7f;

    static constexpr float JUMP_FORCE = -15.0f;

public:

    static constexpr int FRAME_WIDTH = 64;

    static constexpr int FRAME_HEIGHT = 64;

    static constexpr int SCALE = 2;

    static constexpr int WIDTH =
        FRAME_WIDTH * SCALE;

    static constexpr int HEIGHT =
        FRAME_HEIGHT * SCALE;
};