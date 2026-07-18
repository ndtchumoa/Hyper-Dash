#include "scenes/MenuScene.h"

#include "scenes/PlayScene.h"

#include "engine/Game.h"
#include "resources/ResourceManager.h"
#include "resources/AudioID.h"
#include "entities/PlayerSkin.h"
#include "systems/SaveSystem.h"

#include <string>
#include <iostream>

MenuScene::MenuScene(Game& game)
    : Scene(game)
{
}

void MenuScene::init()
{
    SDL_Renderer*    renderer = game.getRenderer();
    ResourceManager& res      = game.getResources();

    // Nhạc menu độc lập với UI — phát trước, không phụ thuộc vào
    // việc load font có thành công hay không.
    game.getAudio().playMusic(MusicID::Menu);

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

    //----------------------------------------------------------
    // High score — đọc từ SaveSystem, hiển thị dưới tiêu đề.
    // SaveSystem chỉ dùng 1 lần ở đây (load-only), không giữ instance.
    //----------------------------------------------------------

    SaveSystem save;
    const SaveData data = save.load();

    const std::string highScoreText =
        "High Score: " + std::to_string(data.high_score);

    m_highScoreLabel.init(
        renderer,
        fontRegular,
        highScoreText,
        { 200, 200, 200, 255 });

    {
        const SDL_Rect b = m_highScoreLabel.getBounds();
        m_highScoreLabel.setPosition(
            (kWindowWidth - b.w) / 2,
            kWindowHeight / 2 - 60);
    }

    //----------------------------------------------------------
    // Character selection (F5) — đọc skin đã chọn từ SaveData,
    // hiển thị tên + 2 nút Prev/Next để đổi. Ghi ngay xuống SaveData
    // mỗi lần đổi (xem changeSkin()) — không cần nút "Save" riêng.
    //----------------------------------------------------------

    m_skinIndex = PlayerSkin::indexOf(data.selected_skin);

    m_skinLabel.init(
        renderer,
        fontRegular,
        std::string(PlayerSkin::kAll[m_skinIndex].displayName),
        { 220, 220, 255, 255 });

    {
        const SDL_Rect b = m_skinLabel.getBounds();
        m_skinLabel.setPosition((kWindowWidth - b.w) / 2, kSkinRowY);
    }

    {
        constexpr int kArrowPaddingX = 16;
        constexpr int kArrowPaddingY = 8;
        constexpr int kArrowGap      = 20;

        int arrowTextW = 0;
        int arrowTextH = 0;
        TTF_SizeText(fontRegular, "<", &arrowTextW, &arrowTextH);

        const int arrowBtnW = arrowTextW + kArrowPaddingX * 2;

        const SDL_Rect labelBounds = m_skinLabel.getBounds();

        m_skinPrevButton.init(
            renderer,
            fontRegular,
            "<",
            labelBounds.x - kArrowGap - arrowBtnW,
            kSkinRowY - kArrowPaddingY,
            kArrowPaddingX,
            kArrowPaddingY);

        m_skinNextButton.init(
            renderer,
            fontRegular,
            ">",
            labelBounds.x + labelBounds.w + kArrowGap,
            kSkinRowY - kArrowPaddingY,
            kArrowPaddingX,
            kArrowPaddingY);
    }

    // Nút Play
    int textW = 0;
    int textH = 0;
    TTF_SizeText(fontRegular, "Play", &textW, &textH);

    constexpr int kPaddingX = 20;
    constexpr int kPaddingY = 10;

    const int btnW = textW + kPaddingX * 2;
    const int btnX = (kWindowWidth  - btnW) / 2;
    const int btnY = kWindowHeight / 2 + 90;

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
    m_skinPrevButton.handleEvent(event);
    m_skinNextButton.handleEvent(event);
}

void MenuScene::update(float /*deltaTime*/)
{
    if (m_playButton.isClicked())
    {
        m_playButton.resetClick();
        game.getSceneManager().changeScene(
            std::make_unique<PlayScene>(game));
        return;
    }

    if (m_skinPrevButton.isClicked())
    {
        m_skinPrevButton.resetClick();
        changeSkin(-1);
    }

    if (m_skinNextButton.isClicked())
    {
        m_skinNextButton.resetClick();
        changeSkin(+1);
    }
}

void MenuScene::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    m_titleLabel.render(renderer);
    m_highScoreLabel.render(renderer);
    m_skinPrevButton.render(renderer);
    m_skinLabel.render(renderer);
    m_skinNextButton.render(renderer);
    m_playButton.render(renderer);
}

void MenuScene::clean()
{
    m_titleLabel.clean();
    m_highScoreLabel.clean();
    m_skinLabel.clean();
    m_skinPrevButton.clean();
    m_skinNextButton.clean();
    m_playButton.clean();

    std::cout << "[MenuScene] Cleaned.\n";
}

void MenuScene::changeSkin(int direction)
{
    const int count = static_cast<int>(PlayerSkin::kAll.size());

    // Wrap vòng tròn cả 2 hướng — (+count) trước %  để tránh kết quả
    // âm khi direction=-1 và m_skinIndex=0.
    const int next =
        (static_cast<int>(m_skinIndex) + direction + count) % count;

    m_skinIndex = static_cast<std::size_t>(next);

    SDL_Renderer* renderer = game.getRenderer();

    m_skinLabel.setText(
        renderer,
        std::string(PlayerSkin::kAll[m_skinIndex].displayName));

    // Recenter vì độ rộng text label có thể đổi (không đáng kể với
    // "Skin 1".."Skin 5" nhưng vẫn recenter cho đúng nếu displayName
    // sau này dài hơn). Vị trí 2 nút Prev/Next KHÔNG đổi theo — chấp
    // nhận lệch nhẹ nếu displayName thay đổi độ dài đáng kể; nếu cần
    // chính xác tuyệt đối, tính lại vị trí nút giống lúc init().
    const SDL_Rect b = m_skinLabel.getBounds();
    m_skinLabel.setPosition((kWindowWidth - b.w) / 2, kSkinRowY);

    // Ghi ngay xuống SaveData — SaveSystem load-modify-save 1 lần,
    // đúng pattern one-shot instance đã dùng ở nơi khác trong code.
    const SaveSystem save;
    SaveData          data = save.load();
    data.selected_skin     = std::string(PlayerSkin::kAll[m_skinIndex].saveKey);
    save.save(data);
}