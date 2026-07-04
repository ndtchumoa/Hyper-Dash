#include "engine/Game.h"

#include "scenes/PlayScene.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>

Game::Game() = default;

Game::~Game()
{
    clean();
}

bool Game::init()
{
    //==========================
    // SDL
    //==========================

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr
            << "SDL_Init failed: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr
            << "IMG_Init failed: "
            << IMG_GetError()
            << '\n';

        return false;
    }

    //==========================
    // Window
    //==========================

    window = SDL_CreateWindow(
        "Hyper Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr
            << "Failed to create window.\n";

        return false;
    }

    //==========================
    // Renderer
    //==========================

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cerr
            << "Failed to create renderer.\n";

        return false;
    }

    //==========================
    // Asset Manager
    //==========================

    assets.init(renderer);

    // preload
    assets.getTexture(
        "assets/textures/player/Character skin colors/Male Skin1.png");

    assets.getTexture(
        "assets/textures/obstacle/Ores.png");

    //==========================
    // First Scene
    //==========================

    sceneManager.changeScene(
        std::make_unique<PlayScene>(*this));

    running = true;

    std::cout
        << "Engine initialized successfully.\n";

    return true;
    
    //==========================
    // Score
    //==========================
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init failed: "
                << TTF_GetError()
                << '\n';

        return false;
    }
}

void Game::run()
{
    while (running)
    {
        handleEvents();

        update();

        render();
    }
}

void Game::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        sceneManager.handleEvents(event);
    }
}

void Game::update()
{
    sceneManager.update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(
        renderer,
        25,
        25,
        25,
        255);

    SDL_RenderClear(renderer);

    sceneManager.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    assets.clear();

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    IMG_Quit();

    SDL_Quit();

    TTF_Quit();
}