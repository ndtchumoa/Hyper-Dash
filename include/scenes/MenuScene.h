#pragma once

#include "engine/Scene.h"
#include "ui/Label.h"
#include "ui/Button.h"

#include <cstddef>

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

    void changeSkin(int direction);

private:

    Label  m_titleLabel;
    Label  m_highScoreLabel;
    Button m_playButton;

    // Character selection (F5) — Prev/Next đổi skin, ghi ngay xuống
    // SaveData mỗi lần đổi (đọc lại ở PlayScene khi bắt đầu chơi).
    Label  m_skinLabel;
    Button m_skinPrevButton;
    Button m_skinNextButton;
    std::size_t m_skinIndex = 0;

    static constexpr int kWindowWidth  = 1280;
    static constexpr int kWindowHeight = 720;
    static constexpr int kSkinRowY     = kWindowHeight / 2;
};