#pragma once

#include "resources/FontID.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class ResourceManager;

class Score
{
public:

    // font không còn được load trong Score — lấy từ ResourceManager.
    bool init(SDL_Renderer* renderer, ResourceManager& resources);

    void addPoint();

    void reset();

    // render() không còn là const vì có thể rebuild texture.
    void render(SDL_Renderer* renderer);

    void clean();

    int getValue() const;

private:

    void rebuildTexture(SDL_Renderer* renderer);

private:

    int m_value = 0;

    TTF_Font*    m_font        = nullptr;   // không own — thuộc ResourceManager
    SDL_Texture* m_textTexture = nullptr;   // own — tự destroy
    SDL_Rect     m_textRect{};

    bool m_dirty = true;

    static constexpr SDL_Color kColor = { 255, 255, 255, 255 };
};