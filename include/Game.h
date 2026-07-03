#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.h"
#include "Obstacle.h"
#include "Score.h"
#include "Background.h"

enum class GameState
{
    Playing,
    GameOver
};

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
    void reset();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect ground;

    Player player;
    Obstacle obstacle;

    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int GROUND_HEIGHT = 50;
    bool running;

    GameState state;
    Score score;
    Background background;
};