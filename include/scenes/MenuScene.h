#pragma once

#include "engine/Scene.h"
#include "ui/Label.h"
#include "ui/Button.h"

class MenuScene : public Scene
{
public:

    explicit MenuScene(Game& game);

    void init()                               override;
    void handleEvents(const SDL_Event& event) override;
    void update(float deltaTime)              override;
    void render(SDL_Renderer* renderer)       override;
    void clean()                              override;

private:

    Label  m_titleLabel;
    Label  m_highScoreLabel;
    Button m_playButton;

    static constexpr int kWindowWidth  = 1280;
    static constexpr int kWindowHeight = 720;
};