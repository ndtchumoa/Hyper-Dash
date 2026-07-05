#pragma once

#include "graphics/AnimationLibrary.h"
#include "graphics/AnimationState.h"
#include "resources/AnimationID.h"
#include "graphics/AnimationFrame.h"

#include <SDL2/SDL.h>

#include <cstddef>
#include <cstdint>

class Animator
{
public:

    Animator() = default;
    ~Animator() = default;

    //----------------------------------------------------------
    // Library
    //----------------------------------------------------------

    void setLibrary(
        const AnimationLibrary* library);

    const AnimationLibrary*
    getLibrary() const;

    //----------------------------------------------------------
    // Playback
    //----------------------------------------------------------

    bool play(
        AnimationID id,
        bool restart = false);

    void stop();

    void pause();

    void resume();

    void restart();

    //----------------------------------------------------------
    // Update
    //----------------------------------------------------------

    void update(
        std::uint32_t deltaTime);

    //----------------------------------------------------------
    // Speed
    //----------------------------------------------------------

    void setSpeed(
        float speed);

    float getSpeed() const;

    //----------------------------------------------------------
    // Flip
    //----------------------------------------------------------

    void setFlip(
        SDL_RendererFlip flip);

    SDL_RendererFlip
    getFlip() const;

    //----------------------------------------------------------
    // State
    //----------------------------------------------------------

    AnimationPlaybackState
    getPlaybackState() const;

    bool isPlaying() const;

    bool isPaused() const;

    bool hasFinished() const;

    //----------------------------------------------------------
    // Current Animation
    //----------------------------------------------------------

    AnimationID
    getCurrentAnimation() const;

    const AnimationClip*
    getCurrentClip() const;

    //----------------------------------------------------------
    // Current Frame
    //----------------------------------------------------------

    std::size_t
    getCurrentFrameIndex() const;

    const AnimationFrame&
    getCurrentFrame() const;

    //----------------------------------------------------------
    // Rendering Helpers
    //----------------------------------------------------------

    SDL_Texture*
    getCurrentTexture() const;

    const SDL_Rect&
    getCurrentSourceRect() const;

private:

    void resetPlayback(bool resetState = true);

private:

    //----------------------------------------------------------
    // Resources
    //----------------------------------------------------------

    const AnimationLibrary* library = nullptr;

    const AnimationClip* currentClip = nullptr;

    //----------------------------------------------------------
    // Playback
    //----------------------------------------------------------

    AnimationID currentAnimation =
        AnimationID::None;

    AnimationPlaybackState playbackState =
        AnimationPlaybackState::Stopped;

    //----------------------------------------------------------
    // Frame
    //----------------------------------------------------------

    std::size_t currentFrame = 0;

    std::uint32_t elapsedTime = 0;

    //----------------------------------------------------------
    // Playback Settings
    //----------------------------------------------------------

    float playbackSpeed = 1.0f;

    SDL_RendererFlip flip =
        SDL_FLIP_NONE;
};