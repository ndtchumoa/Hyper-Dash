#include "entities/PlayerSkin.h"

namespace PlayerSkin
{
    TextureID resolveTexture(std::string_view saveKey)
    {
        for (const auto& def : kAll)
        {
            if (def.saveKey == saveKey)
                return def.texture;
        }

        return TextureID::MaleSkin1;
    }

    std::size_t indexOf(std::string_view saveKey)
    {
        for (std::size_t i = 0; i < kAll.size(); ++i)
        {
            if (kAll[i].saveKey == saveKey)
                return i;
        }

        return 0;
    }
}
