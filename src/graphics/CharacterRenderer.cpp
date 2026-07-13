#include "graphics/CharacterRenderer.h"

#include "graphics/Animator.h"
#include "graphics/SpriteSheet.h"

#include <algorithm>
#include <iostream>

std::size_t CharacterRenderer::addLayer(
    const SpriteSheet* sheet,
    RenderLayer        layer,
    SDL_Point          offset)
{
    SpriteLayerEntry entry;
    entry.sheet   = sheet;
    entry.layer   = layer;
    entry.offset  = offset;
    entry.visible = true;

    m_layers.push_back(entry);

    // Sort theo RenderLayer để đảm bảo thứ tự vẽ đúng.
    // stable_sort bảo toàn insertion order với các entry cùng layer.
    std::stable_sort(
        m_layers.begin(),
        m_layers.end(),
        [](const SpriteLayerEntry& a, const SpriteLayerEntry& b)
        {
            return static_cast<int>(a.layer) <
                   static_cast<int>(b.layer);
        });

    // Tìm lại index thực của entry sau sort — không thể dùng size()-1
    // vì sort có thể di chuyển entry này đến vị trí khác.
    // So sánh bằng pointer đến sheet + layer để tìm đúng entry.
    for (std::size_t i = 0; i < m_layers.size(); ++i)
    {
        if (m_layers[i].sheet == entry.sheet &&
            m_layers[i].layer == entry.layer)
        {
            return i;
        }
    }

    // Fallback không bao giờ xảy ra — entry vừa push_back chắc chắn tồn tại.
    return m_layers.size() - 1;
}

void CharacterRenderer::setLayerVisible(
    std::size_t index,
    bool        visible)
{
    if (index >= m_layers.size())
    {
        std::cerr
            << "[CharacterRenderer] setLayerVisible: index "
            << index << " out of range.\n";
        return;
    }

    m_layers[index].visible = visible;
}

void CharacterRenderer::setLayerSheet(
    std::size_t        index,
    const SpriteSheet* sheet)
{
    if (index >= m_layers.size())
    {
        std::cerr
            << "[CharacterRenderer] setLayerSheet: index "
            << index << " out of range.\n";
        return;
    }

    m_layers[index].sheet = sheet;
}

void CharacterRenderer::clearLayers()
{
    m_layers.clear();
}

std::size_t CharacterRenderer::layerCount() const
{
    return m_layers.size();
}

void CharacterRenderer::render(
    SDL_Renderer*   renderer,
    const Animator& animator,
    const SDL_Rect& dstRect) const
{
    if (!renderer)
        return;

    // Render khi Playing, Paused, hoặc Finished (hiện frame cuối).
    if (!animator.isPlaying()  &&
        !animator.isPaused()   &&
        !animator.hasFinished())
    {
        return;
    }

    const SDL_Rect&        src  = animator.getCurrentSourceRect();
    const SDL_RendererFlip flip = animator.getFlip();

    for (const auto& entry : m_layers)
    {
        if (!entry.visible || !entry.sheet)
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
            0.0,
            nullptr,
            flip);
    }
}