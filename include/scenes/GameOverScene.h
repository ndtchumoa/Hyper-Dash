#pragma once

#include "engine/Scene.h"
#include "ui/Label.h"
#include "ui/Button.h"

class GameOverScene : public Scene
{
public:

    GameOverScene(Game& game, int finalScore);

    void init()                               override;
    void handleEvents(const SDL_Event& event) override;
    void update(float deltaTime)              override;
    void render(SDL_Renderer* renderer)       override;
    void clean()                              override;

private:

    int  m_finalScore    = 0;
    bool m_isNewHighScore = false;

    Label  m_titleLabel;
    Label  m_scoreLabel;
    Label  m_highScoreLabel;
    Button m_restartButton;
    Button m_menuButton;

    static constexpr int kWindowWidth  = 1280;
    static constexpr int kWindowHeight = 720;
};