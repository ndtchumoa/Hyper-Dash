#include "graphics/Background.h"

#include <cmath>

#include "resources/ResourceManager.h"
#include "resources/TextureID.h"

bool Background::init(ResourceManager& resources)
{
    m_layers.clear();
    m_scrollMultiplier = 1.0f;

    struct LayerCfg
    {
        TextureID id;
        float     baseSpeed;   // pixels/giây khi multiplier = 1
    };

    const LayerCfg configs[] =
    {
        { TextureID::BgLayer5, 12.0f },
        { TextureID::BgLayer4, 24.0f },
        { TextureID::BgLayer3, 42.0f },
        { TextureID::BgLayer2, 60.0f },
        { TextureID::BgLayer1, 84.0f },
    };

    for (const auto& cfg : configs)
    {
        SDL_Texture* tex = resources.get(cfg.id);

        if (!tex)
            continue;

        Layer layer;
        layer.texture   = tex;
        layer.baseSpeed = cfg.baseSpeed;
        layer.scroll    = 0.0f;

        SDL_QueryTexture(
            tex, nullptr, nullptr,
            &layer.width, &layer.height);

        m_layers.push_back(layer);
    }

    return !m_layers.empty();
}

void Background::update(float deltaTime)
{
    for (auto& layer : m_layers)
    {
        // Tốc độ thực = baseSpeed * multiplier.
        // baseSpeed không bao giờ thay đổi — chỉ multiplier thay đổi.
        const float effectiveSpeed =
            layer.baseSpeed * m_scrollMultiplier;

        layer.scroll += effectiveSpeed * deltaTime;

        if (layer.scroll >= static_cast<float>(layer.width))
            layer.scroll -= static_cast<float>(layer.width);
    }
}

void Background::render(SDL_Renderer* renderer) const
{
    for (const auto& layer : m_layers)
    {
        // layer.width (1024px) < window width (1280px).
        // 2 bản không đủ cover khi scroll — cần 3 bản.
        // Dùng round() thay truncation để tránh gap 1 pixel khi wrap.
        const int scrollX = static_cast<int>(std::round(layer.scroll));

        for (int i = 0; i < 3; ++i)
        {
            const SDL_Rect dst =
            {
                -scrollX + i * layer.width,
                0,
                layer.width,
                kRenderHeight
            };

            // Chỉ render bản nào thực sự visible trên màn hình.
            if (dst.x + dst.w > 0 && dst.x < 1280)
                SDL_RenderCopy(renderer, layer.texture, nullptr, &dst);
        }
    }
}

void Background::clean()
{
    // Texture thuộc ResourceManager — không destroy ở đây.
    m_layers.clear();
}

void Background::setScrollMultiplier(float multiplier)
{
    // Clamp tránh multiplier âm hoặc bằng 0 gây background đứng yên.
    m_scrollMultiplier = multiplier > 0.0f ? multiplier : 1.0f;
}

float Background::getScrollMultiplier() const
{
    return m_scrollMultiplier;
}