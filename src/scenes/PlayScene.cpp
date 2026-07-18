#include "scenes/PlayScene.h"

#include "scenes/GameOverScene.h"
#include "scenes/MenuScene.h"

#include "engine/Game.h"
#include "resources/FontID.h"
#include "resources/AudioID.h"
#include "entities/PlayerSkin.h"
#include "systems/SaveSystem.h"

#include <string>
#include <iostream>
#include <random>
#include <array>

namespace
{
    // Chọn ngẫu nhiên 1 trong 3 track InGame mỗi lần vào PlayScene —
    // tăng đa dạng nhạc nền qua các lượt chơi. Nằm ở PlayScene (không
    // phải AudioManager) vì đây là quyết định gameplay ("track nào
    // phù hợp lúc này"), AudioManager chỉ phát cái được bảo phát.
    MusicID pickRandomInGameMusic()
    {
        static std::mt19937 rng{ std::random_device{}() };

        static constexpr std::array<MusicID, 3> kTracks =
        {
            MusicID::InGame1,
            MusicID::InGame2,
            MusicID::InGame3,
        };

        std::uniform_int_distribution<std::size_t> dist(
            0, kTracks.size() - 1);

        return kTracks[dist(rng)];
    }
}

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

    // SaveSystem chỉ dùng 1 lần ở đây (load-only) — đúng pattern
    // one-shot instance đã dùng ở MenuScene/GameOverScene, không giữ
    // state. Skin được chọn ở MenuScene (F5), lưu qua SaveData, đọc
    // lại ở đây — PlayScene và MenuScene không phụ thuộc trực tiếp
    // vào nhau, chỉ gián tiếp qua SaveData trên đĩa.
    const SaveSystem save;
    const SaveData    saveData     = save.load();
    const TextureID   skinTexture  =
        PlayerSkin::resolveTexture(saveData.selected_skin);

    m_background.init(res);
    m_player.init(res, m_ground.y, skinTexture);
    m_obstacleManager.init(res, m_ground.y, m_ground.w);

    m_difficultyManager.reset();

    if (!m_score.init(game.getRenderer(), res))
        std::cerr << "[PlayScene] Score init failed.\n";

    m_score.reset();

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
    m_paused      = false;

    initPauseOverlay();

    // Mix_PlayMusic tự halt track menu đang phát trước đó — không
    // cần tự stopMusic().
    game.getAudio().playMusic(pickRandomInGameMusic());

    std::cout << "[PlayScene] Initialized.\n";
}

void PlayScene::initPauseOverlay()
{
    ResourceManager& res = game.getResources();
    SDL_Renderer* renderer = game.getRenderer();

    TTF_Font* fontLarge   = res.getFont(FontID::UILarge);
    TTF_Font* fontRegular = res.getFont(FontID::UIRegular);

    if (!fontLarge || !fontRegular)
    {
        std::cerr << "[PlayScene] Pause overlay font load failed.\n";
        return;
    }

    m_pauseTitleLabel.init(
        renderer,
        fontLarge,
        "PAUSED",
        { 255, 255, 255, 255 });

    {
        const SDL_Rect b = m_pauseTitleLabel.getBounds();
        m_pauseTitleLabel.setPosition(
            (kWindowWidth - b.w) / 2,
            kWindowHeight / 2 - 140);
    }

    m_resumeButton.init(
        renderer,
        fontRegular,
        "Resume",
        kWindowWidth / 2 - 120,
        kWindowHeight / 2 + 20);

    m_menuButton.init(
        renderer,
        fontRegular,
        "Main Menu",
        kWindowWidth / 2 + 20,
        kWindowHeight / 2 + 20);
}

void PlayScene::handleEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        game.quit();
        return;
    }

    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_ESCAPE)
    {
        // Toggle pause thay vì quit — quit ngay bằng ESC không còn
        // phù hợp khi đã có pause menu với lựa chọn rõ ràng.
        m_paused = !m_paused;
        return;
    }

    if (m_paused)
    {
        // Khi paused, chỉ route event cho overlay buttons.
        // Input gameplay (SPACE để nhảy) bị bỏ qua hoàn toàn.
        m_resumeButton.handleEvent(event);
        m_menuButton.handleEvent(event);
        return;
    }

    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_SPACE)
    {
        if (m_player.jump())
            game.getAudio().playSfx(SfxID::Jump);
    }
}

void PlayScene::update(float deltaTime)
{
    if (m_paused)
    {
        updatePause();
        return;
    }

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

    if (m_player.justLanded())
        game.getAudio().playSfx(SfxID::Landing);

    //----------------------------------------------------------
    // Collision
    //----------------------------------------------------------

    if (m_obstacleManager.checkCollision(m_player.getBounds()))
    {
        game.getAudio().playSfx(SfxID::Collision);

        const int finalScore = m_score.getValue();

        game.getSceneManager().changeScene(
            std::make_unique<GameOverScene>(game, finalScore));

        return;
    }

    //----------------------------------------------------------
    // Score
    //----------------------------------------------------------

    // Không phát SFX ở đây nữa — Score.mp3 giờ phát 1 lần ở
    // GameOverScene khi hiển thị điểm cuối, tránh lặp SFX liên tục
    // mỗi khi vượt obstacle (gây ồn/khó chịu ở tốc độ cao).
    if (m_obstacleManager.checkPassed(m_player.getBounds().x))
        m_score.addPoint();
}

void PlayScene::updatePause()
{
    if (m_resumeButton.isClicked())
    {
        m_resumeButton.resetClick();
        m_paused = false;
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

void PlayScene::applyDifficulty()
{
    const DifficultyState& state = m_difficultyManager.getState();

    m_obstacleManager.setSpeed(state.obstacleSpeed);
    m_obstacleManager.setSpawnInterval(state.spawnInterval);
    m_background.setScrollMultiplier(state.bgScrollMultiplier);

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
    // Gameplay luôn render (kể cả khi paused) — người chơi vẫn thấy
    // được vị trí hiện tại, chỉ đóng băng chuyển động.
    m_background.render(renderer);
    m_obstacleManager.render(renderer);
    m_player.render(renderer);

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &m_ground);

    m_score.render(renderer);
    m_levelLabel.render(renderer);

    if (m_paused)
        renderPauseOverlay(renderer);
}

void PlayScene::renderPauseOverlay(SDL_Renderer* renderer) const
{
    // Nền tối mờ phủ lên toàn bộ gameplay đã render.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);

    const SDL_Rect fullScreen = { 0, 0, kWindowWidth, kWindowHeight };
    SDL_RenderFillRect(renderer, &fullScreen);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    m_pauseTitleLabel.render(renderer);
    m_resumeButton.render(renderer);
    m_menuButton.render(renderer);
}

void PlayScene::clean()
{
    m_background.clean();
    m_score.clean();
    m_levelLabel.clean();
    m_pauseTitleLabel.clean();
    m_resumeButton.clean();
    m_menuButton.clean();

    std::cout << "[PlayScene] Cleaned.\n";
}