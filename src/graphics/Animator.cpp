#include "graphics/Animator.h"

#include "graphics/AnimationClip.h"
#include "graphics/SpriteSheet.h"

#include <algorithm>

void Animator::setLibrary(
    const AnimationLibrary* lib)
{
    library = lib;

    stop();
}

const AnimationLibrary*
Animator::getLibrary() const
{
    return library;
}

bool Animator::play(
    AnimationID id,
    bool restart)
{
    if (!library)
        return false;

    const AnimationClip* clip =
        library->getClip(id);

    if (!clip)
        return false;

    if (currentAnimation == id &&
        playbackState == AnimationPlaybackState::Playing &&
        !restart)
    {
        return true;
    }

    currentAnimation = id;

    currentClip = clip;

    resetPlayback(false);

    playbackState =
        AnimationPlaybackState::Playing;

    return true;
}

void Animator::stop()
{
    currentAnimation = AnimationID::None;

    currentClip = nullptr;

    currentFrame = 0;

    elapsedTime = 0;

    playbackState =
        AnimationPlaybackState::Stopped;
}

void Animator::pause()
{
    if (playbackState ==
        AnimationPlaybackState::Playing)
    {
        playbackState =
            AnimationPlaybackState::Paused;
    }
}

void Animator::resume()
{
    if (playbackState ==
        AnimationPlaybackState::Paused)
    {
        playbackState =
            AnimationPlaybackState::Playing;
    }
}

void Animator::restart()
{
    if (!currentClip)
        return;

    resetPlayback(false);

    playbackState =
        AnimationPlaybackState::Playing;
}

void Animator::update(
    std::uint32_t deltaTime)
{
    if (playbackState !=
        AnimationPlaybackState::Playing)
    {
        return;
    }

    if (!currentClip)
        return;

    if (currentClip->empty())
        return;

    elapsedTime +=
        static_cast<std::uint32_t>(
            deltaTime * playbackSpeed);

    while (elapsedTime >=
           currentClip
           ->getFrame(currentFrame)
           .duration)
    {
        elapsedTime -=
            currentClip
            ->getFrame(currentFrame)
            .duration;

        ++currentFrame;

        if (currentFrame >=
            currentClip
            ->getFrameCount())
        {
            if (currentClip->isLooping())
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame =
                    currentClip
                    ->getFrameCount() - 1;

                playbackState =
                    AnimationPlaybackState::Finished;

                break;
            }
        }
    }
}

void Animator::setSpeed(
    float speed)
{
    playbackSpeed =
        std::max(0.0f, speed);
}

float Animator::getSpeed() const
{
    return playbackSpeed;
}

void Animator::setFlip(
    SDL_RendererFlip f)
{
    flip = f;
}

SDL_RendererFlip
Animator::getFlip() const
{
    return flip;
}

AnimationPlaybackState
Animator::getPlaybackState() const
{
    return playbackState;
}

bool Animator::isPlaying() const
{
    return playbackState ==
        AnimationPlaybackState::Playing;
}

bool Animator::isPaused() const
{
    return playbackState ==
        AnimationPlaybackState::Paused;
}

bool Animator::hasFinished() const
{
    return playbackState ==
        AnimationPlaybackState::Finished;
}

AnimationID
Animator::getCurrentAnimation() const
{
    return currentAnimation;
}

const AnimationClip*
Animator::getCurrentClip() const
{
    return currentClip;
}

std::size_t
Animator::getCurrentFrameIndex() const
{
    return currentFrame;
}

const AnimationFrame&
Animator::getCurrentFrame() const
{
    return currentClip
        ->getFrame(currentFrame);
}

SDL_Texture*
Animator::getCurrentTexture() const
{
    if (!currentClip)
        return nullptr;

    const SpriteSheet* sheet =
        currentClip
        ->getSpriteSheet();

    if (!sheet)
        return nullptr;

    return sheet->getTexture();
}

const SDL_Rect&
Animator::getCurrentSourceRect() const
{
    return currentClip
        ->getFrame(currentFrame)
        .sourceRect;
}

void Animator::resetPlayback(
    bool resetState)
{
    currentFrame = 0;

    elapsedTime = 0;

    if (resetState)
    {
        playbackState =
            AnimationPlaybackState::Stopped;
    }
}