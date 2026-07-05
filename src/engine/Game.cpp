#include "engine/Game.h"

#include "scenes/PlayScene.h"

#include <SDL2/SDL_ttf.h>

#include <iostream>

Game::Game() = default;

Game::~Game()
{
    clean();
}

bool Game::init()
{
    //==============================
    // SDL core
    //==============================

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr
            << "[Game] SDL_Init failed: "
            << SDL_GetError() << '\n';
        return false;
    }

    //==============================
    // SDL_image
    //==============================

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr
            << "[Game] IMG_Init failed: "
            << IMG_GetError() << '\n';
        return false;
    }

    //==============================
    // SDL_ttf
    //==============================

    if (TTF_Init() == -1)
    {
        std::cerr
            << "[Game] TTF_Init failed: "
            << TTF_GetError() << '\n';
        return false;
    }

    //==============================
    // Window
    //==============================

    window = SDL_CreateWindow(
        "Hyper Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "[Game] Failed to create window.\n";
        return false;
    }

    //==============================
    // Renderer
    //==============================

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cerr << "[Game] Failed to create renderer.\n";
        return false;
    }

    //==============================
    // Asset Manager
    //==============================

    assets.init(renderer);

    //==============================
    // First Scene
    //==============================

    sceneManager.changeScene(
        std::make_unique<PlayScene>(*this));

    running   = true;
    lastTicks = SDL_GetTicks();

    std::cout << "[Game] Engine initialized successfully.\n";

    return true;
}

void Game::run()
{
    while (running)
    {
        const std::uint32_t now   = SDL_GetTicks();
        const std::uint32_t diff  = now - lastTicks;
        lastTicks = now;

        // Clamp delta time: tránh spiral of death khi window bị drag / debug pause.
        // Giới hạn tối đa 100ms (~10 FPS) để physics không explode.
        constexpr std::uint32_t MAX_DELTA_MS = 100;
        const float deltaTime =
            static_cast<float>(diff < MAX_DELTA_MS ? diff : MAX_DELTA_MS)
            / 1000.0f;

        handleEvents();
        update(deltaTime);
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

void Game::update(float deltaTime)
{
    sceneManager.update(deltaTime);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);

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

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}