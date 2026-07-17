#pragma once

#include "engine/Scene.h"

#include "graphics/Background.h"
#include "entities/Player.h"
#include "entities/ObstacleManager.h"
#include "systems/DifficultyManager.h"
#include "ui/Score.h"
#include "ui/Label.h"
#include "ui/Button.h"

class PlayScene : public Scene
{
public:

    explicit PlayScene(Game& game);

    void init()                               override;
    void handleEvents(const SDL_Event& event) override;
    void update(float deltaTime)              override;
    void render(SDL_Renderer* renderer)       override;
    void clean()                              override;

private:

    void applyDifficulty();

    void initPauseOverlay();
    void updatePause();
    void renderPauseOverlay(SDL_Renderer* renderer) const;

private:

    Background        m_background;
    Player            m_player;
    ObstacleManager   m_obstacleManager;
    DifficultyManager m_difficultyManager;
    Score             m_score;
    Label             m_levelLabel;

    SDL_Rect m_ground{};

    float m_elapsedTime = 0.0f;
    int   m_lastLevel   = 0;

    //==============================
    // Pause — xử lý nội bộ trong PlayScene, không đổi kiến trúc
    // SceneManager. Khi paused, toàn bộ gameplay update() bị freeze
    // (early-return trước mọi logic vật lý/spawn/collision), chỉ
    // overlay UI được update/render thêm.
    //==============================

    bool m_paused = false;

    Label  m_pauseTitleLabel;
    Button m_resumeButton;
    Button m_menuButton;

    static constexpr int kGroundHeight = 80;
    static constexpr int kWindowWidth  = 1280;
    static constexpr int kWindowHeight = 720;
};