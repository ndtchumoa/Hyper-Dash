#pragma once

#include "graphics/RenderLayer.h"

#include <SDL2/SDL.h>

#include <vector>
#include <cstddef>

class Animator;
class SpriteSheet;

// Một layer sprite đơn lẻ gắn với một SpriteSheet.
// CharacterRenderer vẽ từng layer theo thứ tự RenderLayer,
// tất cả dùng chung sourceRect từ Animator.
struct SpriteLayerEntry
{
    const SpriteSheet* sheet = nullptr;

    // Offset pixel khi render layer này (dùng cho căn chỉnh weapon, helmet...)
    SDL_Point offset{ 0, 0 };

    RenderLayer layer = RenderLayer::Body;

    bool visible = true;
};

// CharacterRenderer chỉ có một trách nhiệm duy nhất:
// Nhận frame hiện tại từ Animator và vẽ các SpriteLayer
// lên renderer theo đúng thứ tự.
//
// Không chứa bất kỳ gameplay logic nào.
// Không biết gì về Player, physics hay input.
class CharacterRenderer
{
public:

    CharacterRenderer() = default;

    //--------------------------------------------------
    // Layer management
    //--------------------------------------------------

    // Thêm một layer texture vào danh sách render.
    // Trả về index để caller có thể tham chiếu sau này.
    std::size_t addLayer(
        const SpriteSheet* sheet,
        RenderLayer layer,
        SDL_Point offset = { 0, 0 });

    // Ẩn/hiện một layer theo index (dùng cho equipment swap).
    void setLayerVisible(std::size_t index, bool visible);

    // Thay thế SpriteSheet của một layer (dùng khi đổi trang phục).
    void setLayerSheet(std::size_t index, const SpriteSheet* sheet);

    void clearLayers();

    std::size_t layerCount() const;

    //--------------------------------------------------
    // Render
    //--------------------------------------------------

    // Vẽ toàn bộ character tại vị trí dstRect.
    // sourceRect được lấy từ Animator (frame hiện tại).
    // flip được lấy từ Animator (hướng nhân vật).
    void render(
        SDL_Renderer* renderer,
        const Animator& animator,
        const SDL_Rect& dstRect) const;

private:

    std::vector<SpriteLayerEntry> layers;
};