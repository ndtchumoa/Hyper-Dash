#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "engine/SceneManager.h"

#include "systems/AssetManager.h"

class Game
{
public:
    Game();
    ~Game();

    bool init();

    void run();

    void clean();

    //==========================
    // Getters
    //==========================

    SDL_Renderer* getRenderer() const
    {
        return renderer;
    }

    SDL_Window* getWindow() const
    {
        return window;
    }

    AssetManager& getAssets()
    {
        return assets;
    }

    SceneManager& getSceneManager()
    {
        return sceneManager;
    }

    bool isRunning() const
    {
        return running;
    }

    void quit()
    {
        running = false;
    }

private:
    void handleEvents();

    void update();

    void render();

private:

    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    bool running = false;

    AssetManager assets;

    //AudioManager audio;

    //InputSystem input;

    SceneManager sceneManager;

    static constexpr int WINDOW_WIDTH = 1280;

    static constexpr int WINDOW_HEIGHT = 720;
};