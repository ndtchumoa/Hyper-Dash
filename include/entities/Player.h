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

    SDL_Rect    getBounds() const;
    PlayerState getState()  const;

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

    AnimationLibrary     m_animLibrary;
    Animator              m_animator;
    AnimationController   m_animController;
    CharacterRenderer     m_characterRenderer;

    SDL_Rect m_dstRect{};

    // Tích lũy vị trí bằng float để tránh pixel drift.
    // m_dstRect.y chỉ dùng cho render, luôn sync từ m_posY.
    float m_posY      = 0.0f;
    float m_velocityY = 0.0f;
    int   m_groundY   = 0;
    bool  m_onGround  = true;

    PlayerState m_state = PlayerState::Run;

public:

    // Kích thước frame THỰC của spritesheet — verify bằng phân tích pixel
    // (đo center-of-mass từng frame, drift ~0px xác nhận đúng):
    //   800px / 10 cols = 80px, 448px / 7 rows = 64px.
    // Trước đây nhầm 64x64 (đoán theo convention phổ biến) khiến việc
    // cắt frame lệch pha liên tục — mỗi frame hiển thị ghép nửa pose này
    // với nửa pose khác, tạo hiệu ứng "chạy A→B rồi giật về A".
    static constexpr int kFrameWidth  = 80;
    static constexpr int kFrameHeight = 64;
    static constexpr int kScale       = 2;
    static constexpr int kWidth       = kFrameWidth  * kScale;
    static constexpr int kHeight      = kFrameHeight * kScale;

private:

    static constexpr float kGravity     = 2600.0f;
    static constexpr float kFallGravity = 2600.0f;
    static constexpr float kJumpForce   = -950.0f;

    static constexpr std::uint32_t kFrameDurationMs = 80;

    // Hitbox — đo trực tiếp từ pixel content trong frame gốc 80x64,
    // lấy bbox rộng nhất trong 3 animation (Jump: X 28-53, Y 20-63)
    // để hitbox ổn định không đổi khi chuyển animation.
    // Sau scale x2: content X≈56-106 (trong 160), Y≈40-126 (trong 128).
    static constexpr int kHitboxInsetX = 54;  // mỗi bên trái/phải
    static constexpr int kHitboxInsetY = 40;  // phía trên (chân chạm đáy)
};