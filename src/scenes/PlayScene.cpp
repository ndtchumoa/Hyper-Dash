#include "scenes/PlayScene.h"

#include "scenes/GameOverScene.h"

#include "engine/Game.h"
#include "resources/FontID.h"

#include <string>
#include <iostream>

PlayScene::PlayScene(Game& game)
    : Scene(game)
{
}

void PlayScene::init()
{
    m_ground =
    {
        0,
        kWindowHeight - kGroundHeight,
        kWindowWidth,
        kGroundHeight
    };

    ResourceManager& res = game.getResources();

    m_background.init(res);
    m_player.init(res, m_ground.y);
    m_obstacleManager.init(res, m_ground.y, m_ground.w);

    m_difficultyManager.reset();

    if (!m_score.init(game.getRenderer(), res))
        std::cerr << "[PlayScene] Score init failed.\n";

    m_score.reset();

    // Level label — góc trên phải
    TTF_Font* fontRegular = res.getFont(FontID::UIRegular);

    if (fontRegular)
    {
        m_levelLabel.init(
            game.getRenderer(),
            fontRegular,
            "Level 1",
            { 200, 200, 200, 255 });

        m_levelLabel.setPosition(kWindowWidth - 140, 20);
    }

    m_elapsedTime = 0.0f;
    m_lastLevel   = 1;

    std::cout << "[PlayScene] Initialized.\n";
}

void PlayScene::handleEvents(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        game.quit();
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE: game.quit();     break;
        case SDLK_SPACE:  m_player.jump(); break;
        default: break;
        }
        break;

    default:
        break;
    }
}

void PlayScene::update(float deltaTime)
{
    m_elapsedTime += deltaTime;

    //----------------------------------------------------------
    // Difficulty — update trước tất cả systems khác
    //----------------------------------------------------------

    m_difficultyManager.update(m_elapsedTime, m_score.getValue());

    applyDifficulty();

    //----------------------------------------------------------
    // Gameplay systems
    //----------------------------------------------------------

    m_background.update(deltaTime);
    m_player.update(deltaTime);
    m_obstacleManager.update(deltaTime);

    //----------------------------------------------------------
    // Collision
    //----------------------------------------------------------

    if (m_obstacleManager.checkCollision(m_player.getBounds()))
    {
        const int finalScore = m_score.getValue();

        game.getSceneManager().changeScene(
            std::make_unique<GameOverScene>(game, finalScore));

        return;
    }

    //----------------------------------------------------------
    // Score
    //----------------------------------------------------------

    if (m_obstacleManager.checkPassed(m_player.getBounds().x))
        m_score.addPoint();
}

void PlayScene::applyDifficulty()
{
    const DifficultyState& state = m_difficultyManager.getState();

    // Truyền state xuống từng system — PlayScene là coordinator,
    // không phải calculator. Calculation nằm trong DifficultyManager.
    m_obstacleManager.setSpeed(state.obstacleSpeed);
    m_obstacleManager.setSpawnInterval(state.spawnInterval);
    m_background.setScrollMultiplier(state.bgScrollMultiplier);

    // Cập nhật level label chỉ khi level thay đổi.
    if (state.level != m_lastLevel)
    {
        m_lastLevel = state.level;

        m_levelLabel.setText(
            game.getRenderer(),
            "Level " + std::to_string(state.level));
    }
}

void PlayScene::render(SDL_Renderer* renderer)
{
    m_background.render(renderer);
    m_obstacleManager.render(renderer);
    m_player.render(renderer);

    // Ground placeholder
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &m_ground);

    // UI trên cùng
    m_score.render(renderer);
    m_levelLabel.render(renderer);
}

void PlayScene::clean()
{
    m_background.clean();
    m_score.clean();
    m_levelLabel.clean();

    std::cout << "[PlayScene] Cleaned.\n";
}