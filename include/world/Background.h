#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class AssetManager;

class Background
{
public:
    bool init(AssetManager& assets);

    void update();

    void render(SDL_Renderer* renderer);

    void clean();

private:

    struct Layer
    {
        SDL_Texture* texture = nullptr;

        float scroll = 0.0f;

        float speed = 0.0f;

        int width = 0;
        int height = 0;
    };

    std::vector<Layer> layers;
};