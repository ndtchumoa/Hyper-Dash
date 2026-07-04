#include "systems/AssetManager.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>

bool AssetManager::init(SDL_Renderer* r)
{
    renderer = r;
    return renderer != nullptr;
}

SDL_Texture* AssetManager::getTexture(const std::string& path)
{
    // Đã load trước đó
    auto it = textureCache.find(path);
    if (it != textureCache.end())
    {
        return it->second;
    }

    // Load texture mới
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());

    if (!texture)
    {
        std::cerr
            << "Failed to load texture: "
            << path
            << '\n';

        std::cerr
            << IMG_GetError()
            << '\n';

        return nullptr;
    }

    textureCache[path] = texture;

    return texture;
}

void AssetManager::clear()
{
    for (auto& pair : textureCache)
    {
        if (pair.second)
        {
            SDL_DestroyTexture(pair.second);
        }
    }

    textureCache.clear();
}

void AssetManager::clean()
{
    clear();
    renderer = nullptr;
}