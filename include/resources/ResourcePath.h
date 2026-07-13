#pragma once

#include "resources/TextureID.h"
#include "resources/FontID.h"

#include <string_view>
#include <cstdint>

namespace ResourcePath
{
    //--------------------------------------------------
    // Texture
    //--------------------------------------------------

    // Trả về đường dẫn file cho TextureID.
    // Trả về string_view rỗng nếu id không hợp lệ.
    std::string_view resolve(TextureID id);

    //--------------------------------------------------
    // Font
    //--------------------------------------------------

    struct FontDef
    {
        std::string_view path;
        int              size = 0;
    };

    // Trả về path + size cho FontID.
    // Trả về FontDef{} (path rỗng, size 0) nếu id không hợp lệ.
    FontDef resolveFont(FontID id);
}