#include "engine/Game.h"

#include "scenes/MenuScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

Game::Game() = default;

Game::~Game()
{
    clean();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "[Game] SDL_Init: " << SDL_GetError() << '\n';
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "[Game] IMG_Init: " << IMG_GetError() << '\n';
        return false;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "[Game] TTF_Init: " << TTF_GetError() << '\n';
        return false;
    }

    m_window = SDL_CreateWindow(
        "Hyper Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kWindowWidth,
        kWindowHeight,
        SDL_WINDOW_SHOWN);

    if (!m_window)
    {
        std::cerr << "[Game] Window: " << SDL_GetError() << '\n';
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer)
    {
        std::cerr << "[Game] Renderer: " << SDL_GetError() << '\n';
        return false;
    }

    if (!m_resources.init(m_renderer))
    {
        std::cerr << "[Game] ResourceManager init failed.\n";
        return false;
    }

    // Game bắt đầu từ MenuScene.
    m_sceneManager.changeScene(
        std::make_unique<MenuScene>(*this));

    m_running   = true;
    m_lastTicks = SDL_GetTicks();

    std::cout << "[Game] Initialized.\n";

    return true;
}

void Game::run()
{
    while (m_running)
    {
        const std::uint32_t now  = SDL_GetTicks();
        const std::uint32_t diff = now - m_lastTicks;
        m_lastTicks = now;

        const float deltaTime =
            static_cast<float>(diff < kMaxDeltaMs ? diff : kMaxDeltaMs)
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
            m_running = false;

        m_sceneManager.handleEvents(event);
    }
}

void Game::update(float deltaTime)
{
    m_sceneManager.update(deltaTime);
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    m_sceneManager.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
    m_resources.clean();

    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}