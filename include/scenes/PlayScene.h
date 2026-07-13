#pragma once

#include "engine/Scene.h"

#include "graphics/Background.h"
#include "entities/Player.h"
#include "entities/ObstacleManager.h"
#include "systems/DifficultyManager.h"
#include "ui/Score.h"
#include "ui/Label.h"

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

private:

    Background        m_background;
    Player            m_player;
    ObstacleManager   m_obstacleManager;
    DifficultyManager m_difficultyManager;
    Score             m_score;
    Label             m_levelLabel;

    SDL_Rect m_ground{};

    // Tổng thời gian chơi trong session hiện tại (giây).
    float m_elapsedTime = 0.0f;

    // Level hiện tại — dùng để detect khi nào label cần update.
    int m_lastLevel = 0;

    static constexpr int kGroundHeight = 80;
    static constexpr int kWindowWidth  = 1280;
    static constexpr int kWindowHeight = 720;
};