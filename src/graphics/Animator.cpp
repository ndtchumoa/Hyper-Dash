#include "graphics/Animator.h"

#include "graphics/AnimationClip.h"
#include "graphics/SpriteSheet.h"

#include <algorithm>
#include <cassert>

void Animator::setLibrary(const AnimationLibrary* library)
{
    m_library = library;
    stop();
}

const AnimationLibrary* Animator::getLibrary() const
{
    return m_library;
}

bool Animator::play(AnimationID id, bool restart)
{
    if (!m_library)
        return false;

    const AnimationClip* clip = m_library->getClip(id);

    if (!clip)
        return false;

    // Đang play đúng clip này rồi, không restart → bỏ qua.
    if (m_currentAnimation == id &&
        m_playbackState == AnimationPlaybackState::Playing &&
        !restart)
    {
        return true;
    }

    m_currentAnimation = id;
    m_currentClip      = clip;

    resetPlayback();

    m_playbackState = AnimationPlaybackState::Playing;

    return true;
}

void Animator::stop()
{
    m_currentAnimation = AnimationID::None;
    m_currentClip      = nullptr;
    m_playbackState    = AnimationPlaybackState::Stopped;
    m_currentFrame     = 0;
    m_elapsedTime      = 0;
}

void Animator::pause()
{
    if (m_playbackState == AnimationPlaybackState::Playing)
        m_playbackState = AnimationPlaybackState::Paused;
}

void Animator::resume()
{
    if (m_playbackState == AnimationPlaybackState::Paused)
        m_playbackState = AnimationPlaybackState::Playing;
}

void Animator::restart()
{
    if (!m_currentClip)
        return;

    resetPlayback();
    m_playbackState = AnimationPlaybackState::Playing;
}

void Animator::update(std::uint32_t deltaTimeMs)
{
    if (m_playbackState != AnimationPlaybackState::Playing)
        return;

    if (!m_currentClip || m_currentClip->empty())
        return;

    m_elapsedTime += static_cast<std::uint32_t>(
        deltaTimeMs * m_playbackSpeed);

    // Advance frame(s) berdasarkan elapsed time.
    while (m_elapsedTime >=
           m_currentClip->getFrame(m_currentFrame).duration)
    {
        m_elapsedTime -=
            m_currentClip->getFrame(m_currentFrame).duration;

        ++m_currentFrame;

        if (m_currentFrame >= m_currentClip->getFrameCount())
        {
            if (m_currentClip->isLooping())
            {
                m_currentFrame = 0;
            }
            else
            {
                // Non-looping clip: kẹt ở frame cuối.
                m_currentFrame =
                    m_currentClip->getFrameCount() - 1;

                m_playbackState =
                    AnimationPlaybackState::Finished;

                break;
            }
        }
    }
}

void Animator::setSpeed(float speed)
{
    m_playbackSpeed = std::max(0.0f, speed);
}

float Animator::getSpeed() const
{
    return m_playbackSpeed;
}

void Animator::setFlip(SDL_RendererFlip flip)
{
    m_flip = flip;
}

SDL_RendererFlip Animator::getFlip() const
{
    return m_flip;
}

AnimationPlaybackState Animator::getPlaybackState() const
{
    return m_playbackState;
}

bool Animator::isPlaying() const
{
    return m_playbackState == AnimationPlaybackState::Playing;
}

bool Animator::isPaused() const
{
    return m_playbackState == AnimationPlaybackState::Paused;
}

bool Animator::hasFinished() const
{
    return m_playbackState == AnimationPlaybackState::Finished;
}

AnimationID Animator::getCurrentAnimation() const
{
    return m_currentAnimation;
}

const AnimationClip* Animator::getCurrentClip() const
{
    return m_currentClip;
}

std::size_t Animator::getCurrentFrameIndex() const
{
    return m_currentFrame;
}

const AnimationFrame& Animator::getCurrentFrame() const
{
    assert(m_currentClip != nullptr);
    return m_currentClip->getFrame(m_currentFrame);
}

SDL_Texture* Animator::getCurrentTexture() const
{
    if (!m_currentClip)
        return nullptr;

    const SpriteSheet* sheet = m_currentClip->getSpriteSheet();

    return sheet ? sheet->getTexture() : nullptr;
}

const SDL_Rect& Animator::getCurrentSourceRect() const
{
    assert(m_currentClip != nullptr);
    return m_currentClip->getFrame(m_currentFrame).sourceRect;
}

void Animator::resetPlayback()
{
    m_currentFrame = 0;
    m_elapsedTime  = 0;
}