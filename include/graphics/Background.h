#pragma once

#include <SDL2/SDL.h>

#include <vector>

class ResourceManager;

class Background
{
public:

    bool init(ResourceManager& resources);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer) const;

    void clean();

    // Nhân tất cả layer speed với multiplier.
    // multiplier = 1.0 → tốc độ gốc.
    // multiplier = 2.5 → nhanh gấp 2.5 lần.
    // Gọi từ PlayScene mỗi frame sau khi DifficultyManager update.
    void setScrollMultiplier(float multiplier);

    float getScrollMultiplier() const;

private:

    struct Layer
    {
        SDL_Texture* texture  = nullptr;
        float        scroll   = 0.0f;   // pixels
        float        baseSpeed = 0.0f;  // pixels/giây (base, không đổi)
        int          width    = 0;
        int          height   = 0;
    };

    std::vector<Layer> m_layers;

    // Multiplier áp lên tất cả layer khi update.
    // Tách baseSpeed và multiplier để dễ reset về tốc độ gốc.
    float m_scrollMultiplier = 1.0f;

    static constexpr int kRenderHeight = 720;
};