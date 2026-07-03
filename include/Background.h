#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <string>

class Background
{
public:
    bool load(SDL_Renderer* renderer);

    void update();
    void render(SDL_Renderer* renderer);

    void clean();

private:

    struct Layer
    {
        SDL_Texture* texture = nullptr;

        float x = 0.0f;

        float speed = 0.0f;

        int width = 0;
        int height = 0;
    };

    std::vector<Layer> layers;
};