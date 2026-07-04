#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <unordered_map>

class AssetManager
{
public:
    bool init(SDL_Renderer* renderer);

    SDL_Texture* getTexture(const std::string& path);

    void clear();
    void clean();

private:
    SDL_Renderer* renderer = nullptr;

    std::unordered_map<std::string, SDL_Texture*> textureCache;
};