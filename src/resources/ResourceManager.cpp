#include "resources/ResourceManager.h"

#include "resources/ResourcePath.h"
#include "resources/ResourceLoader.h"

#include <iostream>

ResourceManager::~ResourceManager()
{
    clean();
}

bool ResourceManager::init(SDL_Renderer* renderer)
{
    if (!renderer)
    {
        std::cerr << "[ResourceManager] Renderer is null.\n";
        return false;
    }

    m_renderer = renderer;
    m_textureCache.fill(nullptr);
    m_fontCache.fill(nullptr);
    m_musicCache.fill(nullptr);
    m_sfxCache.fill(nullptr);

    return true;
}

//======================================================
// Texture
//======================================================

SDL_Texture* ResourceManager::get(TextureID id)
{
    if (id == TextureID::None)
        return nullptr;

    const auto index = static_cast<std::size_t>(id);

    if (m_textureCache[index])
        return m_textureCache[index];

    return loadAndCacheTexture(id);
}

bool ResourceManager::preload(TextureID id)
{
    return get(id) != nullptr;
}

void ResourceManager::unload(TextureID id)
{
    if (id == TextureID::None)
        return;

    auto& slot = m_textureCache[static_cast<std::size_t>(id)];

    if (slot)
    {
        SDL_DestroyTexture(slot);
        slot = nullptr;
    }
}

void ResourceManager::unloadAllTextures()
{
    for (auto& tex : m_textureCache)
    {
        if (tex)
        {
            SDL_DestroyTexture(tex);
            tex = nullptr;
        }
    }
}

bool ResourceManager::isLoaded(TextureID id) const
{
    if (id == TextureID::None)
        return false;

    return m_textureCache[static_cast<std::size_t>(id)] != nullptr;
}

SDL_Texture* ResourceManager::loadAndCacheTexture(TextureID id)
{
    const std::string_view path = ResourcePath::resolve(id);

    if (path.empty())
    {
        std::cerr
            << "[ResourceManager] No path for TextureID "
            << static_cast<int>(id) << '\n';
        return nullptr;
    }

    SDL_Texture* tex =
        ResourceLoader::loadTexture(m_renderer, path);

    if (!tex)
        return nullptr;

    m_textureCache[static_cast<std::size_t>(id)] = tex;

    return tex;
}

//======================================================
// Font
//======================================================

TTF_Font* ResourceManager::getFont(FontID id)
{
    if (id == FontID::None)
        return nullptr;

    const auto index = static_cast<std::size_t>(id);

    if (m_fontCache[index])
        return m_fontCache[index];

    return loadAndCacheFont(id);
}

bool ResourceManager::preloadFont(FontID id)
{
    return getFont(id) != nullptr;
}

void ResourceManager::unloadFont(FontID id)
{
    if (id == FontID::None)
        return;

    auto& slot = m_fontCache[static_cast<std::size_t>(id)];

    if (slot)
    {
        TTF_CloseFont(slot);
        slot = nullptr;
    }
}

void ResourceManager::unloadAllFonts()
{
    for (auto& font : m_fontCache)
    {
        if (font)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }
    }
}

bool ResourceManager::isFontLoaded(FontID id) const
{
    if (id == FontID::None)
        return false;

    return m_fontCache[static_cast<std::size_t>(id)] != nullptr;
}

TTF_Font* ResourceManager::loadAndCacheFont(FontID id)
{
    const auto def = ResourcePath::resolveFont(id);

    if (def.path.empty() || def.size <= 0)
    {
        std::cerr
            << "[ResourceManager] No font def for FontID "
            << static_cast<int>(id) << '\n';
        return nullptr;
    }

    TTF_Font* font =
        ResourceLoader::loadFont(def.path, def.size);

    if (!font)
        return nullptr;

    m_fontCache[static_cast<std::size_t>(id)] = font;

    return font;
}

//======================================================
// Music
//======================================================

Mix_Music* ResourceManager::getMusic(MusicID id)
{
    if (id == MusicID::None)
        return nullptr;

    const auto index = static_cast<std::size_t>(id);

    if (m_musicCache[index])
        return m_musicCache[index];

    return loadAndCacheMusic(id);
}

bool ResourceManager::preloadMusic(MusicID id)
{
    return getMusic(id) != nullptr;
}

void ResourceManager::unloadMusic(MusicID id)
{
    if (id == MusicID::None)
        return;

    auto& slot = m_musicCache[static_cast<std::size_t>(id)];

    if (slot)
    {
        Mix_FreeMusic(slot);
        slot = nullptr;
    }
}

void ResourceManager::unloadAllMusic()
{
    for (auto& music : m_musicCache)
    {
        if (music)
        {
            Mix_FreeMusic(music);
            music = nullptr;
        }
    }
}

bool ResourceManager::isMusicLoaded(MusicID id) const
{
    if (id == MusicID::None)
        return false;

    return m_musicCache[static_cast<std::size_t>(id)] != nullptr;
}

Mix_Music* ResourceManager::loadAndCacheMusic(MusicID id)
{
    const std::string_view path = ResourcePath::resolveMusic(id);

    if (path.empty())
    {
        std::cerr
            << "[ResourceManager] No path for MusicID "
            << static_cast<int>(id) << '\n';
        return nullptr;
    }

    Mix_Music* music = ResourceLoader::loadMusic(path);

    if (!music)
        return nullptr;

    m_musicCache[static_cast<std::size_t>(id)] = music;

    return music;
}

//======================================================
// Sfx
//======================================================

Mix_Chunk* ResourceManager::getSfx(SfxID id)
{
    if (id == SfxID::None)
        return nullptr;

    const auto index = static_cast<std::size_t>(id);

    if (m_sfxCache[index])
        return m_sfxCache[index];

    return loadAndCacheSfx(id);
}

bool ResourceManager::preloadSfx(SfxID id)
{
    return getSfx(id) != nullptr;
}

void ResourceManager::unloadSfx(SfxID id)
{
    if (id == SfxID::None)
        return;

    auto& slot = m_sfxCache[static_cast<std::size_t>(id)];

    if (slot)
    {
        Mix_FreeChunk(slot);
        slot = nullptr;
    }
}

void ResourceManager::unloadAllSfx()
{
    for (auto& chunk : m_sfxCache)
    {
        if (chunk)
        {
            Mix_FreeChunk(chunk);
            chunk = nullptr;
        }
    }
}

bool ResourceManager::isSfxLoaded(SfxID id) const
{
    if (id == SfxID::None)
        return false;

    return m_sfxCache[static_cast<std::size_t>(id)] != nullptr;
}

Mix_Chunk* ResourceManager::loadAndCacheSfx(SfxID id)
{
    const std::string_view path = ResourcePath::resolveSfx(id);

    if (path.empty())
    {
        std::cerr
            << "[ResourceManager] No path for SfxID "
            << static_cast<int>(id) << '\n';
        return nullptr;
    }

    Mix_Chunk* chunk = ResourceLoader::loadSfx(path);

    if (!chunk)
        return nullptr;

    m_sfxCache[static_cast<std::size_t>(id)] = chunk;

    return chunk;
}

//======================================================
// Lifecycle
//======================================================

void ResourceManager::unloadAll()
{
    unloadAllTextures();
    unloadAllFonts();
    unloadAllMusic();
    unloadAllSfx();
}

void ResourceManager::clean()
{
    unloadAll();
    m_renderer = nullptr;
}