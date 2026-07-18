#pragma once

#include <cstdint>

// TextureID định danh từng texture asset trong game.
// Gameplay code chỉ biết ID — không biết đường dẫn file.
// Ánh xạ ID → path nằm hoàn toàn trong ResourcePath.
//
// Quy ước thêm ID mới:
//   1. Thêm enum value vào đây.
//   2. Thêm case tương ứng trong ResourcePath::resolve().
//   Không cần sửa bất kỳ gameplay code nào khác.

enum class TextureID : std::uint16_t
{
    None = 0,

    //==============================
    // Male Character — Skin
    //==============================

    MaleSkin1,
    MaleSkin2,
    MaleSkin3,
    MaleSkin4,
    MaleSkin5,

    //==============================
    // Male Character — Hair
    //==============================

    MaleHair1,
    MaleHair2,
    MaleHair3,

    //==============================
    // Male Character — Clothing
    //==============================

    MaleShirt,
    MalePants,
    MaleShoes,

    //==============================
    // Background layers
    //==============================

    BgLayer1,
    BgLayer2,
    BgLayer3,
    BgLayer4,
    BgLayer5,

    //==============================
    // Obstacles
    //==============================

    ObstacleOre,

    // "Garden Decorations.png" — spritesheet chứa 6 vật thể trang trí
    // riêng biệt (xem ObstacleCatalog::resolve() để lấy sourceRect
    // từng vật). 1 TextureID duy nhất cho cả 6 kind, tránh load trùng.
    ObstacleGarden,

    //==============================
    // UI
    //==============================

    UiButton,

    Count   // sentinel — số lượng ID, không dùng làm ID thực
};