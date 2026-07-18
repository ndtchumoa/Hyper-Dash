#include "entities/ObstacleCatalog.h"

namespace
{
    // Hệ số scale DUY NHẤT áp dụng cho mọi obstacle cắt từ
    // "Garden Decorations.png" — giữ nguyên tỉ lệ gốc từng vật
    // (không bóp méo theo chiều nào). Đây là 1 hằng số duy nhất cần
    // chỉnh nếu obstacle nhìn quá to/nhỏ so với Player khi chạy thử
    // trong game (Player render 160x128 — xem Player::kWidth/kHeight).
    //
    // Không áp dụng cho Ore — Ore giữ nguyên kích thước render cũ
    // (48x64) đã được cân bằng gameplay từ trước, không đổi ở đây để
    // tránh ảnh hưởng ngoài ý muốn tới độ khó đã test.
    constexpr float kGardenScale = 1.0f;

    constexpr int scaled(int nativePixels)
    {
        return static_cast<int>(
            static_cast<float>(nativePixels) * kGardenScale);
    }
}

namespace ObstacleCatalog
{
    ObstacleDef resolve(ObstacleKind kind)
    {
        switch (kind)
        {
            case ObstacleKind::Ore:
                // Ores.png native 53x53 — render 48x64 (kích thước cũ,
                // giữ nguyên để không đổi cảm giác chơi đã cân bằng).
                return ObstacleDef{
                    TextureID::ObstacleOre,
                    SDL_Rect{ 0, 0, 53, 53 },
                    48, 64
                };

            case ObstacleKind::GardenPottedPlant:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 0, 7, 31, 57 },
                    scaled(31), scaled(57)
                };

            case ObstacleKind::GardenSmallBush:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 35, 86, 27, 42 },
                    scaled(27), scaled(42)
                };

            case ObstacleKind::GardenBush:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 65, 74, 29, 54 },
                    scaled(29), scaled(54)
                };

            case ObstacleKind::GardenUrn:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 99, 66, 27, 62 },
                    scaled(27), scaled(62)
                };

            case ObstacleKind::GardenCypress:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 130, 26, 28, 102 },
                    scaled(28), scaled(102)
                };

            case ObstacleKind::GardenTree:
                return ObstacleDef{
                    TextureID::ObstacleGarden,
                    SDL_Rect{ 163, 38, 56, 90 },
                    scaled(56), scaled(90)
                };

            default:
                return ObstacleDef{};
        }
    }
}
