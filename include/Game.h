#pragma once

#include <SDL2/SDL.h>

class Game
{
public:
    Game();
    ~Game();

    bool init();
    void run();
    void clean();

private:
    void handleEvents();
    void update();
    void render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
};