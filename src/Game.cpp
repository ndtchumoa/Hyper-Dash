#include "Game.h"

#include <iostream>

Game::Game()
    : window(nullptr),
      renderer(nullptr),
      running(false),
      state(GameState::Playing)
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << IMG_GetError() << '\n';
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

    obstacle.setGroundY(ground.y - Obstacle::HEIGHT);

    background.load(renderer);

    SDL_SetWindowTitle(window, score.getText().c_str());

    running = true;

    return true;
}

void Game::handleEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
        }

        if (e.type == SDL_KEYDOWN)
        {
            // ESC luôn thoát game
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }

            // Space chỉ hoạt động khi đang chơi
            if (state == GameState::Playing &&
                e.key.keysym.sym == SDLK_SPACE)
            {
                player.jump();
            }

            // R chỉ hoạt động khi Game Over
            if (state == GameState::GameOver &&
                e.key.keysym.sym == SDLK_r)
            {
                reset();
            }
        }
    }
}

void Game::update()
{
    background.update();     

    if (state != GameState::Playing)
        return;

    player.update();
    obstacle.update();

    if (obstacle.hasPassedPlayer(player.getRect().x))
    {
        score.increase();
        SDL_SetWindowTitle(window, score.getText().c_str());
    }

    if (SDL_HasIntersection(
            &player.getRect(),
            &obstacle.getRect()))
    {
        state = GameState::GameOver;
    }
}

void Game::render()
{
    // Sky (xanh nhạt)
    if (state == GameState::Playing)
    {
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 180, 60, 60, 255);
    }

    SDL_RenderClear(renderer);
    background.render(renderer);

    // Ground (xám đậm)
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderFillRect(renderer, &ground);

    // Player (xanh lá) - màu được đặt trong Player::render()
    player.render(renderer);

    // Obstacle (đỏ) - màu được đặt trong Obstacle::render()
    obstacle.render(renderer);

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
    background.clean();   // thêm dòng này

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
}

void Game::reset()
{
    player = Player();
    obstacle = Obstacle();

    player.setGroundY(ground.y - Player::HEIGHT);
    obstacle.setGroundY(ground.y - Obstacle::HEIGHT);

    state = GameState::Playing;

    score.reset();
    SDL_SetWindowTitle(window, score.getText().c_str());

    obstacle.resetPassed();
}