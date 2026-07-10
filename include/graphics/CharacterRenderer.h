#pragma once

#include "graphics/RenderLayer.h"

#include <SDL2/SDL.h>

#include <vector>
#include <cstddef>

class Animator;
class SpriteSheet;

// Một layer texture đơn lẻ.
// CharacterRenderer vẽ từng layer theo thứ tự RenderLayer,
// tất cả dùng chung sourceRect từ Animator.
struct SpriteLayerEntry
{
    const SpriteSheet* sheet  = nullptr;
    SDL_Point          offset = { 0, 0 };
    RenderLayer        layer  = RenderLayer::Body;
    bool               visible = true;
};

// Trách nhiệm duy nhất: nhận frame hiện tại từ Animator
// và vẽ các SpriteLayer lên renderer theo đúng thứ tự.
//
// Không chứa gameplay logic.
// Không biết gì về Player, physics hay input.
class CharacterRenderer
{
public:

    CharacterRenderer() = default;

    //--------------------------------------------------
    // Layer management
    //--------------------------------------------------

    std::size_t addLayer(
        const SpriteSheet* sheet,
        RenderLayer layer,
        SDL_Point offset = { 0, 0 });

    void setLayerVisible(std::size_t index, bool visible);

    void setLayerSheet(std::size_t index,
                       const SpriteSheet* sheet);

    void clearLayers();

    std::size_t layerCount() const;

    //--------------------------------------------------
    // Render
    //--------------------------------------------------

    void render(
        SDL_Renderer* renderer,
        const Animator& animator,
        const SDL_Rect& dstRect) const;

private:

    std::vector<SpriteLayerEntry> m_layers;
};