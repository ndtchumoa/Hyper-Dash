#pragma once

#include "resources/TextureID.h"
#include "resources/FontID.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <array>

// ResourceManager là điểm truy cập duy nhất cho tất cả assets.
// Gameplay code chỉ gọi get(TextureID) hoặc getFont(FontID).
//
// Ownership: ResourceManager own tất cả SDL_Texture* và TTF_Font*.
// Assets bị destroy khi unload hoặc clean() được gọi.
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
    // Lifecycle
    //--------------------------------------------------

    void unloadAll();

    void clean();

private:

    SDL_Texture* loadAndCacheTexture(TextureID id);
    TTF_Font*    loadAndCacheFont(FontID id);

private:

    SDL_Renderer* m_renderer = nullptr;

    std::array<SDL_Texture*,
        static_cast<std::size_t>(TextureID::Count)>
        m_textureCache{};

    std::array<TTF_Font*,
        static_cast<std::size_t>(FontID::Count)>
        m_fontCache{};
};