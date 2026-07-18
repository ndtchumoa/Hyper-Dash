#include "resources/ResourcePath.h"

#include <array>

namespace
{
    //--------------------------------------------------
    // Texture paths
    //--------------------------------------------------

    constexpr std::array<std::string_view,
        static_cast<std::size_t>(TextureID::Count)>
    kTexturePaths =
    {{
        // None = 0
        "",

        // MaleSkin1
        "assets/textures/characters/male/skin/Male Skin1.png",
        // MaleSkin2
        "assets/textures/characters/male/skin/Male Skin2.png",
        // MaleSkin3
        "assets/textures/characters/male/skin/Male Skin3.png",
        // MaleSkin4
        "assets/textures/characters/male/skin/Male Skin4.png",
        // MaleSkin5
        "assets/textures/characters/male/skin/Male Skin5.png",

        // MaleHair1
        "assets/textures/characters/male/hair/Male Hair1.png",
        // MaleHair2
        "assets/textures/characters/male/hair/Male Hair2.png",
        // MaleHair3
        "assets/textures/characters/male/hair/Male Hair3.png",

        // MaleShirt
        "assets/textures/characters/male/clothing/Shirt v2.png",
        // MalePants
        "assets/textures/characters/male/clothing/Pants.png",
        // MaleShoes
        "assets/textures/characters/male/clothing/Shoes.png",

        // BgLayer1
        "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 1.png",
        // BgLayer2
        "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 2.png",
        // BgLayer3
        "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 3.png",
        // BgLayer4
        "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 4.png",
        // BgLayer5
        "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 5.png",

        // ObstacleOre
        "assets/textures/obstacles/Ores.png",
        // ObstacleGarden
        "assets/textures/obstacles/Garden Decorations.png",

        // UiButton
        "assets/textures/ui/button.png",
    }};

    static_assert(
        kTexturePaths.size() ==
            static_cast<std::size_t>(TextureID::Count),
        "kTexturePaths size does not match TextureID::Count.");

    //--------------------------------------------------
    // Font definitions
    //--------------------------------------------------

    using FontDef = ResourcePath::FontDef;

    constexpr std::array<FontDef,
        static_cast<std::size_t>(FontID::Count)>
    kFontDefs =
    {{
        // None = 0
        FontDef{ "", 0 },

        // UIRegular
        FontDef{ "assets/fonts/PixelOperator.ttf", 24 },

        // UILarge
        FontDef{ "assets/fonts/PixelOperator.ttf", 48 },
    }};

    static_assert(
        kFontDefs.size() ==
            static_cast<std::size_t>(FontID::Count),
        "kFontDefs size does not match FontID::Count.");

    //--------------------------------------------------
    // Music paths
    //--------------------------------------------------

    constexpr std::array<std::string_view,
        static_cast<std::size_t>(MusicID::Count)>
    kMusicPaths =
    {{
        // None = 0
        "",

        // Menu
        "assets/audio/music/GameIntro1.mp3",
        // InGame1
        "assets/audio/music/InGame1.mp3",
        // InGame2
        "assets/audio/music/InGame2.mp3",
        // InGame3
        "assets/audio/music/InGame3.mp3",

        // Ghi chú: assets/audio/music/GameIntro2.mp3 tồn tại nhưng
        // chưa được map — dự phòng cho variation menu hoặc stinger
        // game-over trong sprint sau.
    }};

    static_assert(
        kMusicPaths.size() ==
            static_cast<std::size_t>(MusicID::Count),
        "kMusicPaths size does not match MusicID::Count.");

    //--------------------------------------------------
    // Sfx paths
    //--------------------------------------------------

    constexpr std::array<std::string_view,
        static_cast<std::size_t>(SfxID::Count)>
    kSfxPaths =
    {{
        // None = 0
        "",

        // Jump
        "assets/audio/sfx/Jumping.mp3",
        // Landing
        "assets/audio/sfx/JumpLanding.mp3",
        // Score
        "assets/audio/sfx/Score.mp3",
        // Collision — chưa có sfx va chạm riêng, tái dùng Explore.mp3
        // cho tới khi có asset chuyên biệt.
        "assets/audio/sfx/Explore.mp3",

        // Ghi chú: assets/audio/sfx/Running.mp3 tồn tại nhưng chưa
        // được map — dự phòng cho footstep loop liên tục, cần cơ chế
        // quản lý channel riêng (loop trên 1 channel cố định), ngoài
        // scope F3.
    }};

    static_assert(
        kSfxPaths.size() ==
            static_cast<std::size_t>(SfxID::Count),
        "kSfxPaths size does not match SfxID::Count.");
}

namespace ResourcePath
{
    std::string_view resolve(TextureID id)
    {
        const auto index = static_cast<std::size_t>(id);

        if (index >= kTexturePaths.size())
            return {};

        return kTexturePaths[index];
    }

    FontDef resolveFont(FontID id)
    {
        const auto index = static_cast<std::size_t>(id);

        if (index >= kFontDefs.size())
            return {};

        return kFontDefs[index];
    }

    std::string_view resolveMusic(MusicID id)
    {
        const auto index = static_cast<std::size_t>(id);

        if (index >= kMusicPaths.size())
            return {};

        return kMusicPaths[index];
    }

    std::string_view resolveSfx(SfxID id)
    {
        const auto index = static_cast<std::size_t>(id);

        if (index >= kSfxPaths.size())
            return {};

        return kSfxPaths[index];
    }
}