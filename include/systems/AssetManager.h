#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <unordered_map>

class AssetManager
{
public:

    AssetManager()  = default;
    ~AssetManager();

    bool init(SDL_Renderer* renderer);

    //--------------------------------------------------
    // Texture API
    //--------------------------------------------------

    bool loadTexture(const std::string& id,
                     const std::string& path);

    SDL_Texture* getTexture(const std::string& id) const;

    bool hasTexture(const std::string& id) const;

    void unloadTexture(const std::string& id);

    void clearTextures();

    //--------------------------------------------------
    // Path-based convenience (load on demand + cache)
    //--------------------------------------------------

    SDL_Texture* getTextureByPath(const std::string& path);

    //--------------------------------------------------

    void clear();

    void clean();

private:

    SDL_Renderer* m_renderer = nullptr;

    // id → texture
    std::unordered_map<std::string, SDL_Texture*> m_textureCache;

    // path → id (để getTextureByPath tra cứu nhanh)
    std::unordered_map<std::string, std::string> m_pathLookup;
};