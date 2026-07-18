#pragma once

#include <cstdint>

// MusicID / SfxID định danh audio asset trong game.
// Gameplay code chỉ biết ID — không biết đường dẫn file.
// Ánh xạ ID → path nằm hoàn toàn trong ResourcePath, giống hệt
// pattern của TextureID/FontID.
//
// Tách 2 enum riêng (thay vì 1 AudioID chung) vì Mix_Music* và
// Mix_Chunk* là 2 kiểu SDL khác nhau, cần 2 cache riêng trong
// ResourceManager — gộp chung sẽ phải runtime-dispatch kiểu, vi phạm
// type safety mà static_assert parity check đang bảo vệ.
//
// Quy ước thêm ID mới:
//   1. Thêm enum value vào đây.
//   2. Thêm case tương ứng trong ResourcePath::resolveMusic() /
//      resolveSfx().
//   Không cần sửa bất kỳ gameplay code nào khác.

enum class MusicID : std::uint8_t
{
    None = 0,

    Menu,       // MenuScene — nhạc nền menu chính
    InGame1,    // PlayScene — 1 trong 3 track được PlayScene chọn ngẫu nhiên
    InGame2,
    InGame3,

    Count       // sentinel — số lượng ID, không dùng làm ID thực
};

enum class SfxID : std::uint8_t
{
    None = 0,

    Jump,       // Player nhảy thành công (Player::jump() trả về true)
    Landing,    // Player chạm đất sau khi nhảy/rơi (Player::justLanded())
    Score,      // Vượt qua 1 obstacle, +1 điểm
    Collision,  // Va chạm obstacle — game over

    Count       // sentinel — số lượng ID, không dùng làm ID thực
};
