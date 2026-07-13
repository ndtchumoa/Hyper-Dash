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
        "assets/textures/background/Ores.png",

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
}