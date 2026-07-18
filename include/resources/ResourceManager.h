#pragma once

#include "resources/TextureID.h"
#include "resources/FontID.h"
#include "resources/AudioID.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <array>

// ResourceManager là điểm truy cập duy nhất cho tất cả assets.
// Gameplay code chỉ gọi get(TextureID), getFont(FontID),
// getMusic(MusicID) hoặc getSfx(SfxID).
//
// Ownership: ResourceManager own tất cả SDL_Texture*, TTF_Font*,
// Mix_Music* và Mix_Chunk*. Assets bị destroy khi unload hoặc
// clean() được gọi. AudioManager KHÔNG own audio asset — nó chỉ giữ
// tham chiếu tới ResourceManager để lấy con trỏ đã cache khi phát.
//
// Cache strategy: load-on-demand, không preload.

class ResourceManager
{
public:

    ResourceManager()  = default;
    ~ResourceManager();

    bool init(SDL_Renderer* renderer);

    //--------------------------------------------------
    // Texture API
    //--------------------------------------------------

    SDL_Texture* get(TextureID id);

    bool preload(TextureID id);

    void unload(TextureID id);

    void unloadAllTextures();

    bool isLoaded(TextureID id) const;

    //--------------------------------------------------
    // Font API
    //--------------------------------------------------

    TTF_Font* getFont(FontID id);

    bool preloadFont(FontID id);

    void unloadFont(FontID id);

    void unloadAllFonts();

    bool isFontLoaded(FontID id) const;

    //--------------------------------------------------
    // Music API
    //--------------------------------------------------

    Mix_Music* getMusic(MusicID id);

    bool preloadMusic(MusicID id);

    void unloadMusic(MusicID id);

    void unloadAllMusic();

    bool isMusicLoaded(MusicID id) const;

    //--------------------------------------------------
    // Sfx API
    //--------------------------------------------------

    Mix_Chunk* getSfx(SfxID id);

    bool preloadSfx(SfxID id);

    void unloadSfx(SfxID id);

    void unloadAllSfx();

    bool isSfxLoaded(SfxID id) const;

    //--------------------------------------------------
    // Lifecycle
    //--------------------------------------------------

    void unloadAll();

    void clean();

private:

    SDL_Texture* loadAndCacheTexture(TextureID id);
    TTF_Font*    loadAndCacheFont(FontID id);
    Mix_Music*   loadAndCacheMusic(MusicID id);
    Mix_Chunk*   loadAndCacheSfx(SfxID id);

private:

    SDL_Renderer* m_renderer = nullptr;

    std::array<SDL_Texture*,
        static_cast<std::size_t>(TextureID::Count)>
        m_textureCache{};

    std::array<TTF_Font*,
        static_cast<std::size_t>(FontID::Count)>
        m_fontCache{};

    std::array<Mix_Music*,
        static_cast<std::size_t>(MusicID::Count)>
        m_musicCache{};

    std::array<Mix_Chunk*,
        static_cast<std::size_t>(SfxID::Count)>
        m_sfxCache{};
};