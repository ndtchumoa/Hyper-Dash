#include "systems/AssetManager.h"

#include <iostream>

AssetManager::~AssetManager()
{
    clean();
}

bool AssetManager::init(SDL_Renderer* r)
{
    renderer = r;
    return renderer != nullptr;
}

bool AssetManager::loadTexture(const std::string& id,
                               const std::string& path)
{
    if (!renderer)
    {
        std::cerr << "[AssetManager] Renderer not initialized.\n";
        return false;
    }

    if (hasTexture(id))
    {
        return true;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());

    if (!texture)
    {
        std::cerr
            << "[AssetManager] Failed to load texture:\n"
            << "ID   : " << id << '\n'
            << "Path : " << path << '\n'
            << "SDL  : " << IMG_GetError() << "\n";

        return false;
    }

    textureCache[id] = texture;
    pathLookup[path] = id;

    return true;
}

SDL_Texture* AssetManager::getTexture(const std::string& id)
{
    auto it = textureCache.find(id);

    if (it == textureCache.end())
        return nullptr;

    return it->second;
}

bool AssetManager::hasTexture(const std::string& id) const
{
    return textureCache.find(id) != textureCache.end();
}

void AssetManager::unloadTexture(const std::string& id)
{
    auto it = textureCache.find(id);

    if (it == textureCache.end())
        return;

    SDL_DestroyTexture(it->second);

    textureCache.erase(it);

    for (auto itr = pathLookup.begin(); itr != pathLookup.end();)
    {
        if (itr->second == id)
            itr = pathLookup.erase(itr);
        else
            ++itr;
    }
}

SDL_Texture* AssetManager::getTextureByPath(const std::string& path)
{
    auto lookup = pathLookup.find(path);

    if (lookup != pathLookup.end())
    {
        return getTexture(lookup->second);
    }

    if (!loadTexture(path, path))
    {
        return nullptr;
    }

    return getTexture(path);
}

void AssetManager::clearTextures()
{
    for (auto& texture : textureCache)
    {
        if (texture.second)
        {
            SDL_DestroyTexture(texture.second);
        }
    }

    textureCache.clear();
    pathLookup.clear();
}

void AssetManager::clear()
{
    clearTextures();
}

void AssetManager::clean()
{
    clearTextures();
    renderer = nullptr;
}