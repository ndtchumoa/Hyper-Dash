#include "scenes/GameOverScene.h"

#include "scenes/PlayScene.h"
#include "scenes/MenuScene.h"

#include "engine/Game.h"
#include "resources/ResourceManager.h"
#include "systems/SaveSystem.h"

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

    //----------------------------------------------------------
    // Load save, so sánh & cập nhật high score nếu cần.
    // SaveSystem không giữ state — tạo instance tạm dùng 1 lần.
    //----------------------------------------------------------

    SaveSystem save;
    SaveData   data = save.load();

    data.total_runs_played += 1;

    if (m_finalScore > data.high_score)
    {
        data.high_score  = m_finalScore;
        m_isNewHighScore = true;
    }

    save.save(data);

    //----------------------------------------------------------
    // "GAME OVER"
    //----------------------------------------------------------

    m_titleLabel.init(
        renderer,
        fontLarge,
        "GAME OVER",
        { 220, 50, 50, 255 });

    {
        const SDL_Rect b = m_titleLabel.getBounds();
        m_titleLabel.setPosition(
            (kWindowWidth  - b.w) / 2,
            kWindowHeight / 2 - 160);
    }

    //----------------------------------------------------------
    // Score cuối
    //----------------------------------------------------------

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
            kWindowHeight / 2 - 90);
    }

    //----------------------------------------------------------
    // High score — "New High Score!" nếu vừa phá kỷ lục,
    // ngược lại hiển thị kỷ lục hiện tại để người chơi biết mục tiêu.
    //----------------------------------------------------------

    const std::string highScoreText = m_isNewHighScore
        ? "New High Score!"
        : ("High Score: " + std::to_string(data.high_score));

    const SDL_Color highScoreColor = m_isNewHighScore
        ? SDL_Color{ 255, 215, 0, 255 }   // vàng gold — nổi bật
        : SDL_Color{ 180, 180, 180, 255 };

    m_highScoreLabel.init(
        renderer,
        fontRegular,
        highScoreText,
        highScoreColor);

    {
        const SDL_Rect b = m_highScoreLabel.getBounds();
        m_highScoreLabel.setPosition(
            (kWindowWidth - b.w) / 2,
            kWindowHeight / 2 - 40);
    }

    //----------------------------------------------------------
    // Buttons
    //----------------------------------------------------------

    m_restartButton.init(
        renderer,
        fontRegular,
        "Restart",
        kWindowWidth / 2 - 120,
        kWindowHeight / 2 + 20);

    m_menuButton.init(
        renderer,
        fontRegular,
        "Main Menu",
        kWindowWidth / 2 + 20,
        kWindowHeight / 2 + 20);

    std::cout
        << "[GameOverScene] Initialized. finalScore=" << m_finalScore
        << ", newHighScore=" << m_isNewHighScore << '\n';
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
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    m_titleLabel.render(renderer);
    m_scoreLabel.render(renderer);
    m_highScoreLabel.render(renderer);
    m_restartButton.render(renderer);
    m_menuButton.render(renderer);
}

void GameOverScene::clean()
{
    m_titleLabel.clean();
    m_scoreLabel.clean();
    m_highScoreLabel.clean();
    m_restartButton.clean();
    m_menuButton.clean();

    std::cout << "[GameOverScene] Cleaned.\n";
}