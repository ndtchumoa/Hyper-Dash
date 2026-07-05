#pragma once

#include "graphics/Animator.h"
#include "graphics/CharacterRenderer.h"
#include "graphics/AnimationLibrary.h"
#include "graphics/SpriteSheet.h"
#include "resources/AnimationID.h"

#include <SDL2/SDL.h>

class AssetManager;

enum class PlayerState
{
    Idle,
    Run,
    Jump,
    Fall
};

// Player xử lý: input response, physics, state machine, animation control.
// Player KHÔNG biết gì về SDL_RenderCopy — đó là trách nhiệm của CharacterRenderer.
class Player
{
public:

    bool init(AssetManager& assets, int groundY);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer);

    void jump();

    void reset();

    SDL_Rect getBounds() const;

    PlayerState getState() const { return state; }

private:

    void updatePhysics(float deltaTime);

    void updateState();

    void updateAnimation();

    bool buildAnimationLibrary(AssetManager& assets);

private:

    //==============================
    // Sprite Sheets (một per texture layer)
    //==============================

    SpriteSheet skinSheet;
    SpriteSheet shirtSheet;
    SpriteSheet pantsSheet;
    SpriteSheet hairSheet;
    SpriteSheet shoesSheet;

    //==============================
    // Animation pipeline
    //==============================

    AnimationLibrary animLibrary;

    Animator animator;

    CharacterRenderer characterRenderer;

    //==============================
    // Physics
    //==============================

    SDL_Rect dstRect{};

    float velocityY = 0.0f;

    int groundY = 0;

    bool onGround = true;

    //==============================
    // State
    //==============================

    PlayerState state = PlayerState::Run;

    //==============================
    // Constants
    //==============================

public:

    static constexpr int FRAME_WIDTH  = 64;
    static constexpr int FRAME_HEIGHT = 64;
    static constexpr int SCALE        = 2;
    static constexpr int WIDTH        = FRAME_WIDTH  * SCALE;
    static constexpr int HEIGHT       = FRAME_HEIGHT * SCALE;

private:

    static constexpr float GRAVITY    = 980.0f;  // pixels/s^2 (variable timestep)
    static constexpr float JUMP_FORCE = -520.0f; // pixels/s   (variable timestep)
};