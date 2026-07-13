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

    //==============================
    // UI
    //==============================

    UiButton,

    Count   // sentinel — số lượng ID, không dùng làm ID thực
};