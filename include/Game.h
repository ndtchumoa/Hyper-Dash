#pragma once

#include <SDL2/SDL.h>
#include "Player.h"
#include "Obstacle.h"

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
    SDL_Rect ground;

private:
    Player player;
    Obstacle obstacle;

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int GROUND_HEIGHT = 50;
    bool running;
};