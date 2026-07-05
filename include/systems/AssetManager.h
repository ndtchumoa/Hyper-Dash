#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <unordered_map>

class AssetManager
{
public:

    AssetManager() = default;
    ~AssetManager();

    bool init(SDL_Renderer* renderer);

    //--------------------------------------
    // Texture Management (New API)
    //--------------------------------------

    bool loadTexture(const std::string& id,
                     const std::string& path);

    SDL_Texture* getTexture(const std::string& id);

    bool hasTexture(const std::string& id) const;

    void unloadTexture(const std::string& id);

    void clearTextures();

    //--------------------------------------
    // Legacy API (Backward Compatibility)
    //--------------------------------------

    SDL_Texture* getTextureByPath(const std::string& path);

    //--------------------------------------

    void clear();
    void clean();

private:

    SDL_Renderer* renderer = nullptr;

    // id -> texture
    std::unordered_map<std::string, SDL_Texture*> textureCache;

    // path -> id
    std::unordered_map<std::string, std::string> pathLookup;
};