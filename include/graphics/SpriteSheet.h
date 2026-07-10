#pragma once

#include <SDL2/SDL.h>

#include <vector>

class SpriteSheet
{
public:

    SpriteSheet() = default;

    // Tạo sheet bằng cách cắt đều theo grid (frameWidth x frameHeight).
    bool create(
        SDL_Texture* texture,
        int frameWidth,
        int frameHeight);

    // Tạo sheet từ danh sách rects tuỳ chỉnh (cho sprite không đều).
    bool create(
        SDL_Texture* texture,
        const std::vector<SDL_Rect>& frames);

    SDL_Texture* getTexture() const;

    const SDL_Rect& getFrame(std::size_t index) const;

    std::size_t getFrameCount() const;

    int getFrameWidth() const;

    int getFrameHeight() const;

    bool empty() const;

    void clear();

private:

    void generateGridFrames();

private:

    SDL_Texture* m_texture     = nullptr;

    int m_textureWidth  = 0;
    int m_textureHeight = 0;

    int m_frameWidth    = 0;
    int m_frameHeight   = 0;

    std::vector<SDL_Rect> m_frames;
};