#pragma once

#include "resources/AudioID.h"

#include <SDL2/SDL_mixer.h>

class ResourceManager;

// AudioManager điều phối phát nhạc nền và sound effect.
//
// KHÔNG sở hữu Mix_Music*/Mix_Chunk* — toàn bộ audio asset được load
// và cache trong ResourceManager (giống Texture/Font), AudioManager
// chỉ giữ tham chiếu tới ResourceManager để lấy asset khi cần phát.
//
// Trách nhiệm duy nhất: gọi Mix_PlayMusic/Mix_PlayChannel đúng lúc
// và quản lý volume. AudioManager KHÔNG biết gì về gameplay (không tự
// quyết định khi nào nên phát SFX nào, hay chọn track nào) — quyết
// định đó thuộc về Scene/Entity gọi vào đây. Giữ đúng nguyên tắc đã
// áp dụng cho Animator/AnimationController: "gameplay decides WHICH
// state should be active", audio chỉ phát cái được bảo phát.
class AudioManager
{
public:

    AudioManager() = default;

    // Yêu cầu Mix_OpenAudio đã được gọi trước đó (Game::init()).
    bool init(ResourceManager& resources);

    //--------------------------------------------------
    // Music — chỉ 1 track phát cùng lúc (giới hạn của SDL_mixer).
    //--------------------------------------------------

    void playMusic(MusicID id, bool loop = true);

    void stopMusic();

    bool isMusicPlaying() const;

    //--------------------------------------------------
    // Sfx — phát trên channel bất kỳ còn trống, có thể chồng lấp.
    //--------------------------------------------------

    void playSfx(SfxID id);

    //--------------------------------------------------
    // Volume — thang 0..MIX_MAX_VOLUME (128).
    //--------------------------------------------------

    void setMusicVolume(int volume);
    void setSfxVolume(int volume);

    int getMusicVolume() const { return m_musicVolume; }
    int getSfxVolume()   const { return m_sfxVolume;   }

    // Dừng phát và bỏ tham chiếu ResourceManager. KHÔNG gọi
    // Mix_CloseAudio/Mix_Quit ở đây — đó là vòng đời thiết bị audio,
    // thuộc trách nhiệm của Game (mở/đóng đối xứng với Mix_Init).
    void clean();

private:

    ResourceManager* m_resources = nullptr;

    int m_musicVolume = kDefaultVolume;
    int m_sfxVolume   = kDefaultVolume;

    // ~62% MIX_MAX_VOLUME — mức mặc định hợp lý, tránh giật tai khi
    // chưa có UI chỉnh volume (F5+).
    static constexpr int kDefaultVolume = 80;
};
