#include "Game.h"

#include <iostream>

Game::Game()
    : window(nullptr),
      renderer(nullptr),
      running(false)
{
}

Game::~Game()
{
    clean();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }

    window = SDL_CreateWindow(
        "Hyper Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: "
                << SDL_GetError() << '\n';
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: "
                << SDL_GetError() << '\n';
        return false;
    }

    ground = {
        0,
        WINDOW_HEIGHT - GROUND_HEIGHT,
        WINDOW_WIDTH,
        GROUND_HEIGHT
    };

    player.setGroundY(ground.y - Player::HEIGHT);

    running = true;

    return true;
}

void Game::handleEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            running = false;

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_SPACE)
                player.jump();
        }
    }
}

void Game::update()
{
    player.update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &ground);

    player.render(renderer);

    SDL_RenderPresent(renderer);
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

void Game::clean()
{
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

    SDL_Quit();
}