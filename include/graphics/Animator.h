#pragma once

#include "graphics/AnimationLibrary.h"
#include "graphics/AnimationState.h"
#include "graphics/AnimationFrame.h"
#include "resources/AnimationID.h"

#include <SDL2/SDL.h>

#include <cstddef>
#include <cstdint>

class Animator
{
public:

    Animator()  = default;
    ~Animator() = default;

    //----------------------------------------------------------
    // Library
    //----------------------------------------------------------

    void setLibrary(const AnimationLibrary* library);

    const AnimationLibrary* getLibrary() const;

    //----------------------------------------------------------
    // Playback
    //----------------------------------------------------------

    bool play(AnimationID id, bool restart = false);

    void stop();

    void pause();

    void resume();

    void restart();

    //----------------------------------------------------------
    // Update
    //----------------------------------------------------------

    void update(std::uint32_t deltaTimeMs);

    //----------------------------------------------------------
    // Settings
    //----------------------------------------------------------

    void setSpeed(float speed);

    float getSpeed() const;

    void setFlip(SDL_RendererFlip flip);

    SDL_RendererFlip getFlip() const;

    //----------------------------------------------------------
    // State queries
    //----------------------------------------------------------

    AnimationPlaybackState getPlaybackState() const;

    bool isPlaying()   const;
    bool isPaused()    const;
    bool hasFinished() const;

    //----------------------------------------------------------
    // Current animation
    //----------------------------------------------------------

    AnimationID          getCurrentAnimation() const;
    const AnimationClip* getCurrentClip()      const;

    //----------------------------------------------------------
    // Current frame
    //----------------------------------------------------------

    std::size_t          getCurrentFrameIndex() const;
    const AnimationFrame& getCurrentFrame()     const;

    //----------------------------------------------------------
    // Rendering helpers
    //----------------------------------------------------------

    // Texture của clip hiện tại (lấy từ SpriteSheet gắn vào clip).
    SDL_Texture* getCurrentTexture() const;

    // sourceRect của frame hiện tại — dùng bởi CharacterRenderer.
    const SDL_Rect& getCurrentSourceRect() const;

private:

    void resetPlayback();

private:

    const AnimationLibrary* m_library     = nullptr;
    const AnimationClip*    m_currentClip = nullptr;

    AnimationID            m_currentAnimation = AnimationID::None;
    AnimationPlaybackState m_playbackState    = AnimationPlaybackState::Stopped;

    std::size_t   m_currentFrame = 0;
    std::uint32_t m_elapsedTime  = 0;

    float            m_playbackSpeed = 1.0f;
    SDL_RendererFlip m_flip          = SDL_FLIP_NONE;
};