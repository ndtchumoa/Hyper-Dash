#include "entities/Player.h"

#include "systems/AssetManager.h"
#include "resources/TextureID.h"

#include <iostream>

// ─────────────────────────────────────────────────────────────
//  Helpers — xây AnimationClip từ một spritesheet grid
//  Row 0 = Idle (13 frames), Row 1 = Run (13 frames),
//  Row 2 = Jump (13 frames)  — theo spritesheet male character
//  Mỗi frame: 64x64px, duration 80ms
// ─────────────────────────────────────────────────────────────

namespace
{
    // Tạo clip từ một hàng cụ thể trên spritesheet.
    // sheet phải đã được create() thành công trước khi gọi hàm này.
    AnimationClip makeRowClip(
        const SpriteSheet& sheet,
        int row,
        int frameCount,
        std::uint32_t frameDurationMs,
        bool loop)
    {
        AnimationClip clip;
        clip.setSpriteSheet(&sheet);
        clip.setLoop(loop);

        const int cols = static_cast<int>(sheet.getFrameCount())
                       / (sheet.getFrameHeight() > 0
                          ? 1
                          : 1); // guard

        // Sheet đã tự cắt theo grid — frame index = row * cols + col
        // Tổng số cột = textureWidth / frameWidth (được tính bởi SpriteSheet)
        // Chúng ta cần số cột; tính lại từ frameCount đã biết.
        for (int col = 0; col < frameCount; ++col)
        {
            const std::size_t frameIndex =
                static_cast<std::size_t>(row * frameCount + col);

            if (frameIndex >= sheet.getFrameCount())
            {
                std::cerr
                    << "[Player] makeRowClip: frameIndex "
                    << frameIndex << " out of range ("
                    << sheet.getFrameCount() << " frames total).\n";
                break;
            }

            AnimationFrame frame;
            frame.sourceRect = sheet.getFrame(frameIndex);
            frame.duration   = frameDurationMs;

            clip.addFrame(frame);
        }

        return clip;
    }
}

// ─────────────────────────────────────────────────────────────

bool Player::init(AssetManager& assets, int ground)
{
    groundY = ground;

    //----------------------------------------------------------
    // 1. Load textures và build SpriteSheets
    //    Mỗi layer (skin, shirt, pants, hair, shoes) dùng
    //    cùng layout frame — đây là quy ước của bộ asset này.
    //----------------------------------------------------------

    struct LayerDef
    {
        const char*   texID;
        SpriteSheet*  sheet;
    };

    const LayerDef layerDefs[] =
    {
        { TextureID::MALE_SKIN_1,  &skinSheet  },
        { TextureID::MALE_SHIRT,   &shirtSheet },
        { TextureID::MALE_PANTS,   &pantsSheet },
        { TextureID::MALE_HAIR_1,  &hairSheet  },
        { TextureID::MALE_SHOES,   &shoesSheet },
    };

    for (const auto& def : layerDefs)
    {
        SDL_Texture* tex = assets.getTextureByPath(def.texID);

        if (!tex)
        {
            std::cerr
                << "[Player] Failed to load texture: "
                << def.texID << '\n';
            return false;
        }

        if (!def.sheet->create(tex, FRAME_WIDTH, FRAME_HEIGHT))
        {
            std::cerr
                << "[Player] Failed to create SpriteSheet for: "
                << def.texID << '\n';
            return false;
        }
    }

    //----------------------------------------------------------
    // 2. Build AnimationLibrary
    //----------------------------------------------------------

    if (!buildAnimationLibrary(assets))
        return false;

    //----------------------------------------------------------
    // 3. Setup Animator
    //----------------------------------------------------------

    animator.setLibrary(&animLibrary);
    animator.play(AnimationID::PlayerRun);

    //----------------------------------------------------------
    // 4. Setup CharacterRenderer — thứ tự layer theo RenderLayer enum
    //    Body < Clothing (shirt) < Clothing (pants) < Hair < ...
    //----------------------------------------------------------

    characterRenderer.clearLayers();
    characterRenderer.addLayer(&skinSheet,  RenderLayer::Body);
    characterRenderer.addLayer(&pantsSheet, RenderLayer::Clothing);
    characterRenderer.addLayer(&shirtSheet, RenderLayer::Clothing);
    characterRenderer.addLayer(&shoesSheet, RenderLayer::Clothing);
    characterRenderer.addLayer(&hairSheet,  RenderLayer::Hair);

    //----------------------------------------------------------
    // 5. Vị trí ban đầu
    //----------------------------------------------------------

    dstRect = { 120, groundY - HEIGHT, WIDTH, HEIGHT };
    velocityY = 0.0f;
    onGround  = true;
    state     = PlayerState::Run;

    return true;
}

bool Player::buildAnimationLibrary(AssetManager& /*assets*/)
{
    // Spritesheet male character layout:
    //   Row 0 — Idle   : 13 frames
    //   Row 1 — Run    : 13 frames
    //   Row 2 — Jump   : 13 frames
    //
    // Tất cả layer (skin/shirt/pants/hair/shoes) dùng cùng layout.
    // Animation data được build từ skinSheet vì tất cả sheet có cùng frame grid.

    constexpr int   FRAMES_PER_ROW   = 13;
    constexpr std::uint32_t FRAME_MS = 80; // ms/frame ~ 12.5 fps

    struct ClipDef
    {
        AnimationID id;
        int         row;
        int         count;
        std::uint32_t duration;
        bool        loop;
    };

    const ClipDef clipDefs[] =
    {
        { AnimationID::PlayerIdle,  0, FRAMES_PER_ROW, FRAME_MS,      true  },
        { AnimationID::PlayerRun,   1, FRAMES_PER_ROW, FRAME_MS,      true  },
        { AnimationID::PlayerJump,  2, FRAMES_PER_ROW, FRAME_MS,      false },
    };

    for (const auto& def : clipDefs)
    {
        AnimationClip clip = makeRowClip(
            skinSheet,       // chỉ dùng để lấy frame rects; SpriteSheet khác dùng chung rect
            def.row,
            def.count,
            def.duration,
            def.loop);

        if (clip.empty())
        {
            std::cerr
                << "[Player] buildAnimationLibrary: clip empty for AnimationID "
                << static_cast<int>(def.id) << '\n';
            return false;
        }

        // Mỗi clip cần biết SpriteSheet của chính nó để CharacterRenderer
        // lấy đúng texture. Nhưng CharacterRenderer lấy texture từ SpriteLayerEntry,
        // không từ clip — clip chỉ cần sourceRect. Vì tất cả layer có cùng grid,
        // việc setSpriteSheet(&skinSheet) ở đây đủ để Animator trả về sourceRect đúng.
        clip.setSpriteSheet(&skinSheet);

        animLibrary.addClip(def.id, clip);
    }

    return true;
}

// ─────────────────────────────────────────────────────────────

void Player::update(float deltaTime)
{
    updatePhysics(deltaTime);
    updateState();
    updateAnimation();

    animator.update(
        static_cast<std::uint32_t>(deltaTime * 1000.0f));
}

void Player::updatePhysics(float deltaTime)
{
    // Variable timestep: velocityY tính bằng pixels/giây
    velocityY += GRAVITY * deltaTime;

    dstRect.y += static_cast<int>(velocityY * deltaTime);

    if (dstRect.y >= groundY - HEIGHT)
    {
        dstRect.y = groundY - HEIGHT;
        velocityY = 0.0f;
        onGround  = true;
    }
}

void Player::updateState()
{
    if (!onGround)
    {
        state = (velocityY < 0.0f)
            ? PlayerState::Jump
            : PlayerState::Fall;
    }
    else
    {
        state = PlayerState::Run;
    }
}

void Player::updateAnimation()
{
    // Chuyển state → AnimationID và gọi animator.play().
    // Animator tự xử lý việc không restart nếu clip đang chạy.
    switch (state)
    {
    case PlayerState::Idle:
        animator.play(AnimationID::PlayerIdle);
        break;

    case PlayerState::Run:
        animator.play(AnimationID::PlayerRun);
        break;

    case PlayerState::Jump:
    case PlayerState::Fall:
        // Jump clip không loop — khi hết nó ở trạng thái Finished,
        // animator.play() với restart=false sẽ không làm gì thêm.
        animator.play(AnimationID::PlayerJump);
        break;
    }
}

// ─────────────────────────────────────────────────────────────

void Player::render(SDL_Renderer* renderer)
{
    characterRenderer.render(renderer, animator, dstRect);
}

void Player::jump()
{
    if (!onGround)
        return;

    velocityY = JUMP_FORCE;
    onGround  = false;

    // Restart jump animation từ đầu mỗi lần nhảy.
    animator.play(AnimationID::PlayerJump, /*restart=*/true);
}

void Player::reset()
{
    dstRect   = { 120, groundY - HEIGHT, WIDTH, HEIGHT };
    velocityY = 0.0f;
    onGround  = true;
    state     = PlayerState::Run;

    animator.play(AnimationID::PlayerRun, /*restart=*/true);
}

SDL_Rect Player::getBounds() const
{
    return dstRect;
}