#include "entities/Player.h"

#include "resources/ResourceManager.h"
#include "resources/TextureID.h"

#include <iostream>

// buildRowClip cắt frameCount frame từ một row cụ thể của spritesheet.
// stride = số cột THỰC của sheet (sheet.getColumnCount()), không phải
// frameCount của clip — hai giá trị này khác nhau khi các animation có
// số frame khác nhau trên cùng 1 sheet (vd: Idle=5, Run=8, Jump=8 nhưng
// sheet có 10 cột/hàng).
static AnimationClip buildRowClip(
    const SpriteSheet& sheet,
    int                row,
    int                frameCount,
    std::uint32_t      frameDurationMs,
    bool               loop)
{
    AnimationClip clip;
    clip.setSpriteSheet(&sheet);
    clip.setLoop(loop);

    const int stride = sheet.getColumnCount();

    for (int col = 0; col < frameCount; ++col)
    {
        const auto index =
            static_cast<std::size_t>(row * stride + col);

        if (index >= sheet.getFrameCount())
        {
            std::cerr
                << "[Player] buildRowClip: frameIndex "
                << index << " out of range.\n";
            break;
        }

        AnimationFrame frame;
        frame.sourceRect = sheet.getFrame(index);
        frame.duration   = frameDurationMs;
        clip.addFrame(frame);
    }

    return clip;
}

bool Player::init(ResourceManager& resources, int groundY)
{
    m_groundY = groundY;

    //----------------------------------------------------------
    // 1. Load textures
    //----------------------------------------------------------

    struct LayerDef
    {
        TextureID    id;
        SpriteSheet* sheet;
    };

    const LayerDef layerDefs[] =
    {
        { TextureID::MaleSkin1, &m_skinSheet  },
        { TextureID::MaleShirt, &m_shirtSheet },
        { TextureID::MalePants, &m_pantsSheet },
        { TextureID::MaleHair1, &m_hairSheet  },
        { TextureID::MaleShoes, &m_shoesSheet },
    };

    for (const auto& def : layerDefs)
    {
        SDL_Texture* tex = resources.get(def.id);

        if (!tex)
        {
            std::cerr
                << "[Player] Failed to get TextureID "
                << static_cast<int>(def.id) << '\n';
            return false;
        }

        if (!def.sheet->create(tex, kFrameWidth, kFrameHeight))
        {
            std::cerr
                << "[Player] Failed to create SpriteSheet for TextureID "
                << static_cast<int>(def.id) << '\n';
            return false;
        }
    }

    //----------------------------------------------------------
    // 2. Build AnimationLibrary
    //----------------------------------------------------------

    if (!buildAnimationLibrary())
        return false;

    //----------------------------------------------------------
    // 3. Setup Animator
    //    Spritesheet hướng ngược chiều game → flip horizontal.
    //----------------------------------------------------------

    m_animator.setLibrary(&m_animLibrary);
    m_animator.setFlip(SDL_FLIP_HORIZONTAL);
    m_animator.play(AnimationID::PlayerRun);

    //----------------------------------------------------------
    // 4. Setup AnimationController
    //----------------------------------------------------------

    m_animController.setAnimator(&m_animator);

    //----------------------------------------------------------
    // 5. Setup CharacterRenderer
    //----------------------------------------------------------

    m_characterRenderer.clearLayers();
    m_characterRenderer.addLayer(&m_skinSheet,  RenderLayer::Body);
    m_characterRenderer.addLayer(&m_pantsSheet, RenderLayer::Clothing);
    m_characterRenderer.addLayer(&m_shirtSheet, RenderLayer::Clothing);
    m_characterRenderer.addLayer(&m_shoesSheet, RenderLayer::Clothing);
    m_characterRenderer.addLayer(&m_hairSheet,  RenderLayer::Hair);

    //----------------------------------------------------------
    // 6. Initial position
    //----------------------------------------------------------

    m_dstRect   = { 120, m_groundY - kHeight, kWidth, kHeight };
    m_posY      = static_cast<float>(m_groundY - kHeight);
    m_velocityY = 0.0f;
    m_onGround  = true;
    m_state     = PlayerState::Run;

    return true;
}

bool Player::buildAnimationLibrary()
{
    // Frame count đo trực tiếp từ spritesheet bằng phân tích pixel:
    // mỗi row có số pose thực khác nhau, phần còn lại của row là ô rỗng.
    struct ClipDef
    {
        AnimationID id;
        int         row;
        int         frameCount;
        bool        loop;
    };

    const ClipDef clipDefs[] =
    {
        { AnimationID::PlayerIdle, 0, 5, true  },
        { AnimationID::PlayerWalk, 1, 8, true  }, // nếu sau này dùng
        { AnimationID::PlayerRun,  2, 8, true  },
        { AnimationID::PlayerJump, 3, 8, false },
        { AnimationID::PlayerFall, 4, 8, false },
    };

    for (const auto& def : clipDefs)
    {
        AnimationClip clip = buildRowClip(
            m_skinSheet,
            def.row,
            def.frameCount,
            kFrameDurationMs,
            def.loop);

        if (clip.empty())
        {
            std::cerr
                << "[Player] Empty clip for AnimationID "
                << static_cast<int>(def.id) << '\n';
            return false;
        }

        m_animLibrary.addClip(def.id, clip);
    }

    return true;
}

void Player::update(float deltaTime)
{
    updatePhysics(deltaTime);
    updateState();
    m_animController.update(m_state);
    m_animator.update(
        static_cast<std::uint32_t>(deltaTime * 1000.0f));
}

void Player::updatePhysics(float deltaTime)
{
    const float gravity = (m_velocityY >= 0.0f)
        ? kFallGravity
        : kGravity;

    m_velocityY += gravity * deltaTime;

    // Tích lũy float để tránh pixel drift — cast int mỗi frame
    // làm mất phần thập phân, gây giật vị trí.
    m_posY += m_velocityY * deltaTime;

    const float groundPosY = static_cast<float>(m_groundY - kHeight);

    if (m_posY >= groundPosY)
    {
        m_posY      = groundPosY;
        m_velocityY = 0.0f;
        m_onGround  = true;
    }

    m_dstRect.y = static_cast<int>(m_posY);
}

void Player::updateState()
{
    if (!m_onGround)
    {
        m_state = (m_velocityY < 0.0f)
            ? PlayerState::Jump
            : PlayerState::Fall;
    }
    else
    {
        m_state = PlayerState::Run;
    }
}

void Player::render(SDL_Renderer* renderer) const
{
    m_characterRenderer.render(renderer, m_animator, m_dstRect);
}

void Player::jump()
{
    if (!m_onGround)
        return;

    m_velocityY = kJumpForce;
    m_onGround  = false;
}

void Player::reset()
{
    m_dstRect   = { 120, m_groundY - kHeight, kWidth, kHeight };
    m_posY      = static_cast<float>(m_groundY - kHeight);
    m_velocityY = 0.0f;
    m_onGround  = true;
    m_state     = PlayerState::Run;

    m_animator.play(AnimationID::PlayerRun, true);
}

SDL_Rect Player::getBounds() const
{
    // Hitbox nhỏ hơn dstRect, loại bỏ transparent padding.
    return SDL_Rect
    {
        m_dstRect.x + kHitboxInsetX,
        m_dstRect.y + kHitboxInsetY,
        m_dstRect.w - kHitboxInsetX * 2,
        m_dstRect.h - kHitboxInsetY
    };
}

PlayerState Player::getState() const
{
    return m_state;
}