#include "scenes/GameOverScene.h"

#include "scenes/PlayScene.h"
#include "scenes/MenuScene.h"

#include "engine/Game.h"
#include "resources/ResourceManager.h"

#include <string>
#include <iostream>

GameOverScene::GameOverScene(Game& game, int finalScore)
    : Scene(game)
    , m_finalScore(finalScore)
{
}

void GameOverScene::init()
{
    SDL_Renderer*    renderer = game.getRenderer();
    ResourceManager& res      = game.getResources();

    TTF_Font* fontLarge   = res.getFont(FontID::UILarge);
    TTF_Font* fontRegular = res.getFont(FontID::UIRegular);

    if (!fontLarge || !fontRegular)
    {
        std::cerr << "[GameOverScene] Font load failed.\n";
        return;
    }

    // "GAME OVER" — căn giữa theo chiều ngang
    m_titleLabel.init(
        renderer,
        fontLarge,
        "GAME OVER",
        { 220, 50, 50, 255 });

    {
        const SDL_Rect b = m_titleLabel.getBounds();
        m_titleLabel.setPosition(
            (kWindowWidth  - b.w) / 2,
            kWindowHeight / 2 - 140);
    }

    // Score cuối
    const std::string scoreText =
        "Score: " + std::to_string(m_finalScore);

    m_scoreLabel.init(
        renderer,
        fontRegular,
        scoreText,
        { 255, 255, 255, 255 });

    {
        const SDL_Rect b = m_scoreLabel.getBounds();
        m_scoreLabel.setPosition(
            (kWindowWidth - b.w) / 2,
            kWindowHeight / 2 - 60);
    }

    // Nút Restart
    m_restartButton.init(
        renderer,
        fontRegular,
        "Restart",
        kWindowWidth / 2 - 120,
        kWindowHeight / 2 + 20);

    // Nút Menu
    m_menuButton.init(
        renderer,
        fontRegular,
        "Main Menu",
        kWindowWidth / 2 + 20,
        kWindowHeight / 2 + 20);

    std::cout << "[GameOverScene] Initialized.\n";
}

void GameOverScene::handleEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        game.quit();
        return;
    }

    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_ESCAPE)
    {
        game.quit();
        return;
    }

    m_restartButton.handleEvent(event);
    m_menuButton.handleEvent(event);
}

void GameOverScene::update(float /*deltaTime*/)
{
    if (m_restartButton.isClicked())
    {
        m_restartButton.resetClick();
        game.getSceneManager().changeScene(
            std::make_unique<PlayScene>(game));
        return;
    }

    if (m_menuButton.isClicked())
    {
        m_menuButton.resetClick();
        game.getSceneManager().changeScene(
            std::make_unique<MenuScene>(game));
        return;
    }
}

void GameOverScene::render(SDL_Renderer* renderer)
{
    // Nền tối mờ
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    m_titleLabel.render(renderer);
    m_scoreLabel.render(renderer);
    m_restartButton.render(renderer);
    m_menuButton.render(renderer);
}

void GameOverScene::clean()
{
    m_titleLabel.clean();
    m_scoreLabel.clean();
    m_restartButton.clean();
    m_menuButton.clean();

    std::cout << "[GameOverScene] Cleaned.\n";
}