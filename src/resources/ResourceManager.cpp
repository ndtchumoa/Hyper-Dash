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
// Lifecycle
//======================================================

void ResourceManager::unloadAll()
{
    unloadAllTextures();
    unloadAllFonts();
}

void ResourceManager::clean()
{
    unloadAll();
    m_renderer = nullptr;
}