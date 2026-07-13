#include "graphics/Background.h"

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
        { TextureID::BgLayer1, 12.0f },
        { TextureID::BgLayer2, 24.0f },
        { TextureID::BgLayer3, 42.0f },
        { TextureID::BgLayer4, 60.0f },
        { TextureID::BgLayer5, 84.0f },
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
        const int scrollX = static_cast<int>(layer.scroll);

        const SDL_Rect dst1 =
        {
            -scrollX,
            0,
            layer.width,
            kRenderHeight
        };

        const SDL_Rect dst2 =
        {
            -scrollX + layer.width,
            0,
            layer.width,
            kRenderHeight
        };

        SDL_RenderCopy(renderer, layer.texture, nullptr, &dst1);
        SDL_RenderCopy(renderer, layer.texture, nullptr, &dst2);
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