#pragma once

#include <cstdint>

// FontID định danh từng font asset trong game.
// Gameplay code chỉ biết ID — không biết đường dẫn file.
//
// Quy ước thêm font mới:
//   1. Thêm enum value vào đây.
//   2. Thêm entry tương ứng trong ResourcePath (FontSizePair).
//   Không cần sửa bất kỳ gameplay code nào khác.

enum class FontID : std::uint8_t
{
    None = 0,

    // Font chính — dùng cho score, tiêu đề, nút
    UIRegular,    // size 24
    UILarge,      // size 48 — tiêu đề GameOver / Menu

    Count
};