#include "graphics/CharacterRenderer.h"

#include "graphics/Animator.h"
#include "graphics/SpriteSheet.h"

#include <algorithm>
#include <cassert>
#include <iostream>

std::size_t CharacterRenderer::addLayer(
    const SpriteSheet* sheet,
    RenderLayer layer,
    SDL_Point offset)
{
    SpriteLayerEntry entry;
    entry.sheet   = sheet;
    entry.layer   = layer;
    entry.offset  = offset;
    entry.visible = true;

    layers.push_back(entry);

    // Sort by RenderLayer enum value so Body < Clothing < Hair < Weapon etc.
    // Stable sort preserves insertion order for entries with equal layer.
    std::stable_sort(
        layers.begin(),
        layers.end(),
        [](const SpriteLayerEntry& a, const SpriteLayerEntry& b)
        {
            return static_cast<int>(a.layer) <
                   static_cast<int>(b.layer);
        });

    return layers.size() - 1;
}

void CharacterRenderer::setLayerVisible(
    std::size_t index,
    bool visible)
{
    if (index >= layers.size())
    {
        std::cerr
            << "[CharacterRenderer] setLayerVisible: index "
            << index << " out of range.\n";
        return;
    }

    layers[index].visible = visible;
}

void CharacterRenderer::setLayerSheet(
    std::size_t index,
    const SpriteSheet* sheet)
{
    if (index >= layers.size())
    {
        std::cerr
            << "[CharacterRenderer] setLayerSheet: index "
            << index << " out of range.\n";
        return;
    }

    layers[index].sheet = sheet;
}

void CharacterRenderer::clearLayers()
{
    layers.clear();
}

std::size_t CharacterRenderer::layerCount() const
{
    return layers.size();
}

void CharacterRenderer::render(
    SDL_Renderer* renderer,
    const Animator& animator,
    const SDL_Rect& dstRect) const
{
    if (!renderer)
        return;

    if (!animator.isPlaying() && !animator.isPaused())
        return;

    const SDL_Rect& src = animator.getCurrentSourceRect();

    const SDL_RendererFlip flip = animator.getFlip();

    for (const auto& entry : layers)
    {
        if (!entry.visible)
            continue;

        if (!entry.sheet)
            continue;

        SDL_Texture* tex = entry.sheet->getTexture();

        if (!tex)
            continue;

        SDL_Rect dst = dstRect;
        dst.x += entry.offset.x;
        dst.y += entry.offset.y;

        SDL_RenderCopyEx(
            renderer,
            tex,
            &src,
            &dst,
            0.0,        // angle
            nullptr,    // center (nullptr = center of dst)
            flip);
    }
}