#pragma once

#include "resources/TextureID.h"

#include <array>
#include <cstddef>
#include <string_view>

// PlayerSkin là cầu nối DUY NHẤT giữa SaveData.selected_skin (string,
// để dễ đọc/debug trong file JSON) và TextureID (dùng bởi
// ResourceManager/Player). Không nơi nào khác trong code được tự ý
// suy diễn saveKey ↔ TextureID — luôn đi qua đây, tránh 2 nguồn sự
// thật lệch nhau khi thêm/bớt skin.
//
// Đã verify cả 5 file Male Skin1-5.png cùng kích thước 800x448
// (cùng layout 80x64 frame với Skin1) — an toàn dùng chung 1 pipeline
// SpriteSheet/AnimationClip trong Player, không cần xử lý riêng.
namespace PlayerSkin
{
    struct SkinDef
    {
        TextureID        texture;
        std::string_view saveKey;      // giá trị lưu trong SaveData.selected_skin
        std::string_view displayName;  // hiển thị trên UI chọn skin (MenuScene)
    };

    // Danh sách tất cả skin khả dụng, thứ tự cố định — dùng cho UI
    // Previous/Next (index +1/-1 rồi wrap qua kAll.size()).
    inline constexpr std::array<SkinDef, 5> kAll =
    {{
        { TextureID::MaleSkin1, "MaleSkin1", "Skin 1" },
        { TextureID::MaleSkin2, "MaleSkin2", "Skin 2" },
        { TextureID::MaleSkin3, "MaleSkin3", "Skin 3" },
        { TextureID::MaleSkin4, "MaleSkin4", "Skin 4" },
        { TextureID::MaleSkin5, "MaleSkin5", "Skin 5" },
    }};

    // Trả về TextureID cho saveKey đã lưu. Trả về MaleSkin1 (default
    // an toàn) nếu saveKey không khớp skin nào — xảy ra khi save file
    // cũ/hỏng hoặc field trống, không throw/crash.
    TextureID resolveTexture(std::string_view saveKey);

    // Index trong kAll của saveKey hiện tại. Trả về 0 (Skin 1) nếu
    // không khớp — dùng làm điểm bắt đầu an toàn cho UI Previous/Next.
    std::size_t indexOf(std::string_view saveKey);
}
