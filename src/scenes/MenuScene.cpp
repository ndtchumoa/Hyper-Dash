#include "scenes/MenuScene.h"

#include "scenes/PlayScene.h"

#include "engine/Game.h"
#include "resources/ResourceManager.h"

#include <iostream>

MenuScene::MenuScene(Game& game)
    : Scene(game)
{
}

void MenuScene::init()
{
    SDL_Renderer*    renderer = game.getRenderer();
    ResourceManager& res      = game.getResources();

    TTF_Font* fontLarge   = res.getFont(FontID::UILarge);
    TTF_Font* fontRegular = res.getFont(FontID::UIRegular);

    if (!fontLarge || !fontRegular)
    {
        std::cerr << "[MenuScene] Font load failed.\n";
        return;
    }

    // Tiêu đề game
    m_titleLabel.init(
        renderer,
        fontLarge,
        "HYPER DASH",
        { 255, 200, 0, 255 });

    {
        const SDL_Rect b = m_titleLabel.getBounds();
        m_titleLabel.setPosition(
            (kWindowWidth  - b.w) / 2,
            kWindowHeight / 2 - 120);
    }

    // Nút Play — TTF_SizeText để tính kích thước trước,
    // tránh gọi init() hai lần chỉ để lấy bounds.
    int textW = 0;
    int textH = 0;
    TTF_SizeText(fontRegular, "Play", &textW, &textH);

    constexpr int kPaddingX = 20;
    constexpr int kPaddingY = 10;

    const int btnW = textW + kPaddingX * 2;
    const int btnX = (kWindowWidth  - btnW) / 2;
    const int btnY = kWindowHeight / 2 + 20;

    m_playButton.init(
        renderer,
        fontRegular,
        "Play",
        btnX,
        btnY,
        kPaddingX,
        kPaddingY);

    std::cout << "[MenuScene] Initialized.\n";
}

void MenuScene::handleEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        game.quit();
        return;
    }

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            game.quit();
            break;

        // Phím Space / Enter cũng start game
        case SDLK_SPACE:
        case SDLK_RETURN:
            game.getSceneManager().changeScene(
                std::make_unique<PlayScene>(game));
            return;

        default:
            break;
        }
    }

    m_playButton.handleEvent(event);
}

void MenuScene::update(float /*deltaTime*/)
{
    if (m_playButton.isClicked())
    {
        m_playButton.resetClick();
        game.getSceneManager().changeScene(
            std::make_unique<PlayScene>(game));
    }
}

void MenuScene::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    m_titleLabel.render(renderer);
    m_playButton.render(renderer);
}

void MenuScene::clean()
{
    m_titleLabel.clean();
    m_playButton.clean();

    std::cout << "[MenuScene] Cleaned.\n";
}