#include "systems/AudioManager.h"

#include "resources/ResourceManager.h"

#include <algorithm>
#include <iostream>

bool AudioManager::init(ResourceManager& resources)
{
    m_resources = &resources;

    // Áp dụng volume mặc định (hoặc volume đã set trước init, nếu
    // Game gọi setMusicVolume/setSfxVolume từ SaveData sau init()).
    setMusicVolume(m_musicVolume);
    setSfxVolume(m_sfxVolume);

    return true;
}

void AudioManager::playMusic(MusicID id, bool loop)
{
    if (!m_resources)
        return;

    Mix_Music* music = m_resources->getMusic(id);

    if (!music)
    {
        std::cerr
            << "[AudioManager] Missing music for MusicID "
            << static_cast<int>(id) << '\n';
        return;
    }

    // Mix_PlayMusic tự halt track hiện tại trước khi phát track mới —
    // an toàn để gọi mỗi lần đổi scene mà không cần tự stopMusic() trước.
    if (Mix_PlayMusic(music, loop ? -1 : 1) == -1)
    {
        std::cerr
            << "[AudioManager] Mix_PlayMusic failed: "
            << Mix_GetError() << '\n';
    }
}

void AudioManager::stopMusic()
{
    Mix_HaltMusic();
}

bool AudioManager::isMusicPlaying() const
{
    return Mix_PlayingMusic() != 0;
}

void AudioManager::playSfx(SfxID id)
{
    if (!m_resources)
        return;

    Mix_Chunk* chunk = m_resources->getSfx(id);

    if (!chunk)
    {
        std::cerr
            << "[AudioManager] Missing sfx for SfxID "
            << static_cast<int>(id) << '\n';
        return;
    }

    // channel = -1: SDL_mixer tự chọn channel trống đầu tiên, cho
    // phép nhiều SFX chồng lấp (vd: Jump + Score cùng lúc) mà không
    // cắt tiếng nhau.
    if (Mix_PlayChannel(-1, chunk, 0) == -1)
    {
        std::cerr
            << "[AudioManager] Mix_PlayChannel failed: "
            << Mix_GetError() << '\n';
    }
}

void AudioManager::setMusicVolume(int volume)
{
    m_musicVolume = std::clamp(volume, 0, MIX_MAX_VOLUME);
    Mix_VolumeMusic(m_musicVolume);
}

void AudioManager::setSfxVolume(int volume)
{
    m_sfxVolume = std::clamp(volume, 0, MIX_MAX_VOLUME);

    // channel = -1: áp dụng cho tất cả channel hiện có.
    Mix_Volume(-1, m_sfxVolume);
}

void AudioManager::clean()
{
    stopMusic();
    m_resources = nullptr;
}
