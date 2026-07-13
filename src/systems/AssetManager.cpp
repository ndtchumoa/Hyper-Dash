#include "systems/AssetManager.h"

#include <iostream>

AssetManager::~AssetManager()
{
    clean();
}

bool AssetManager::init(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    return m_renderer != nullptr;
}

bool AssetManager::loadTexture(
    const std::string& id,
    const std::string& path)
{
    if (!m_renderer)
    {
        std::cerr << "[AssetManager] Renderer not initialized.\n";
        return false;
    }

    // Cache hit — không load lại.
    if (hasTexture(id))
        return true;

    SDL_Texture* texture =
        IMG_LoadTexture(m_renderer, path.c_str());

    if (!texture)
    {
        std::cerr
            << "[AssetManager] Failed to load texture:\n"
            << "  ID   : " << id   << '\n'
            << "  Path : " << path << '\n'
            << "  SDL  : " << IMG_GetError() << '\n';
        return false;
    }

    m_textureCache[id]   = texture;
    m_pathLookup[path]   = id;

    return true;
}

SDL_Texture* AssetManager::getTexture(
    const std::string& id) const
{
    auto it = m_textureCache.find(id);

    return (it != m_textureCache.end())
        ? it->second
        : nullptr;
}

bool AssetManager::hasTexture(const std::string& id) const
{
    return m_textureCache.contains(id);
}

void AssetManager::unloadTexture(const std::string& id)
{
    auto it = m_textureCache.find(id);

    if (it == m_textureCache.end())
        return;

    SDL_DestroyTexture(it->second);
    m_textureCache.erase(it);

    // Xoá luôn path lookup entry tương ứng.
    std::erase_if(
        m_pathLookup,
        [&id](const auto& pair) { return pair.second == id; });
}

SDL_Texture* AssetManager::getTextureByPath(
    const std::string& path)
{
    // Nếu đã load qua path này trước đó → trả về từ cache.
    auto it = m_pathLookup.find(path);

    if (it != m_pathLookup.end())
        return getTexture(it->second);

    // Chưa có → load mới, dùng path làm id.
    if (!loadTexture(path, path))
        return nullptr;

    return getTexture(path);
}

void AssetManager::clearTextures()
{
    for (auto& [id, tex] : m_textureCache)
    {
        if (tex)
            SDL_DestroyTexture(tex);
    }

    m_textureCache.clear();
    m_pathLookup.clear();
}

void AssetManager::clear()
{
    clearTextures();
}

void AssetManager::clean()
{
    clearTextures();
    m_renderer = nullptr;
}