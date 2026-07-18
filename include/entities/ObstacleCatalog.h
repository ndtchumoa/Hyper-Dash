#pragma once

#include "resources/TextureID.h"

#include <SDL2/SDL.h>
#include <cstdint>

// ObstacleKind định danh từng "loại" obstacle có thể spawn.
//
// Nhiều ObstacleKind có thể trỏ vào CÙNG 1 TextureID (spritesheet
// chứa nhiều sprite, cắt bằng sourceRect khác nhau) — giữ đúng
// nguyên tắc chỉ load asset 1 lần qua ResourceManager, không nhân
// bản texture cho từng object nhỏ bên trong 1 file ảnh.
//
// Quy ước thêm kind mới:
//   1. Thêm enum value vào đây.
//   2. Thêm case tương ứng trong ObstacleCatalog::resolve().
//   Không cần sửa Obstacle hay ObstacleManager.
enum class ObstacleKind : std::uint8_t
{
    Ore = 0,

    // "Garden Decorations.png" (224x128px) — 6 vật thể cắt bằng
    // cluster detection trên alpha channel (bounding box thực tế
    // của từng vật, không phải lưới đều — spritesheet đóng gói kiểu
    // atlas). Tên bên dưới mô tả hình dáng quan sát được để dễ tra
    // cứu trong code, không phải tên chính thức của asset — đổi tên
    // thoải mái nếu không khớp khi nhìn trong game.
    GardenPottedPlant,  // 31x57 — chậu cây hoa
    GardenSmallBush,    // 27x42 — bụi cây nhỏ
    GardenBush,         // 29x54 — bụi cây có bệ
    GardenUrn,          // 27x62 — chậu/bình cao
    GardenCypress,      // 28x102 — cây dáng cột, cao mảnh
    GardenTree,         // 56x90 — cây tán tròn, lớn nhất

    Count   // sentinel — số lượng kind, không dùng làm kind thực
};

// Định nghĩa render/collision cho 1 ObstacleKind: texture nguồn,
// vùng cắt (sourceRect) trong texture đó, và kích thước thực tế sẽ
// render lên màn hình (đồng thời cũng là kích thước hitbox — Obstacle
// không có inset riêng, xem Obstacle::getBounds()).
struct ObstacleDef
{
    TextureID texture{};
    SDL_Rect  sourceRect{};
    int       renderWidth  = 0;
    int       renderHeight = 0;
};

namespace ObstacleCatalog
{
    // Trả về định nghĩa cho 1 ObstacleKind.
    // Trả về ObstacleDef{} (texture=None) nếu kind không hợp lệ.
    ObstacleDef resolve(ObstacleKind kind);

    // Tổng số kind hợp lệ — dùng để random chọn kind lúc spawn
    // (ObstacleManager::pickRandomKind()).
    constexpr int count()
    {
        return static_cast<int>(ObstacleKind::Count);
    }
}
