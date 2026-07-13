#pragma once

#include "graphics/Animator.h"
#include "graphics/AnimationController.h"
#include "graphics/CharacterRenderer.h"
#include "graphics/AnimationLibrary.h"
#include "graphics/SpriteSheet.h"
#include "entities/PlayerState.h"

#include <SDL2/SDL.h>

class ResourceManager;

class Player
{
public:

    bool init(ResourceManager& resources, int groundY);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer) const;

    void jump();

    void reset();

    // Trả về hitbox thực (nhỏ hơn dstRect) để collision fair hơn.
    SDL_Rect getBounds() const;

    PlayerState getState() const;

private:

    void updatePhysics(float deltaTime);
    void updateState();
    bool buildAnimationLibrary();

private:

    SpriteSheet m_skinSheet;
    SpriteSheet m_shirtSheet;
    SpriteSheet m_pantsSheet;
    SpriteSheet m_hairSheet;
    SpriteSheet m_shoesSheet;

    AnimationLibrary    m_animLibrary;
    Animator            m_animator;
    AnimationController m_animController;
    CharacterRenderer   m_characterRenderer;

    SDL_Rect m_dstRect{};       // visual rect (render)
    float    m_velocityY = 0.0f;
    int      m_groundY   = 0;
    bool     m_onGround  = true;

    PlayerState m_state = PlayerState::Run;

    //==============================
    // Constants
    //==============================

public:

    static constexpr int kFrameWidth  = 64;
    static constexpr int kFrameHeight = 64;
    static constexpr int kScale       = 2;
    static constexpr int kWidth       = kFrameWidth  * kScale;
    static constexpr int kHeight      = kFrameHeight * kScale;

private:

    // Physics
    static constexpr float kGravity     = 2600.0f;  // rise gravity (pixels/s²)
    static constexpr float kFallGravity = 2600.0f;  // fall gravity — snappier landing
    static constexpr float kJumpForce   = -950.0f;  // pixels/s (âm = đi lên)

    // Animation
    static constexpr int           kFramesPerRow   = 13;
    static constexpr std::uint32_t kFrameDurationMs = 80;

    // Hitbox inset — thu nhỏ so với dstRect để collision fair hơn.
    // Sprite character thực chiếm ~60% chiều rộng, ~85% chiều cao.
    // Transparent padding ở 4 phía bị loại bỏ.
    static constexpr int kHitboxInsetX = 28;  // mỗi bên trái/phải
    static constexpr int kHitboxInsetY = 16;  // phía trên
};