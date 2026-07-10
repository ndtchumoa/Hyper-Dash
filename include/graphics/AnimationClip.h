#pragma once

#include "graphics/AnimationFrame.h"

#include <string>
#include <vector>
#include <cstdint>

class SpriteSheet;

class AnimationClip
{
public:

    AnimationClip() = default;

    explicit AnimationClip(std::string name);

    //-------------------------

    void setName(const std::string& name);

    const std::string& getName() const;

    //-------------------------

    void addFrame(const AnimationFrame& frame);

    const AnimationFrame& getFrame(std::size_t index) const;

    AnimationFrame& getFrame(std::size_t index);

    std::size_t getFrameCount() const;

    bool empty() const;

    void clear();

    //-------------------------

    void setLoop(bool loop);

    bool isLooping() const;

    //-------------------------

    std::uint32_t getTotalDuration() const;

    //-------------------------

    void setSpriteSheet(const SpriteSheet* sheet);

    const SpriteSheet* getSpriteSheet() const;

private:

    std::string m_name;

    std::vector<AnimationFrame> m_frames;

    bool m_loop = true;

    std::uint32_t m_totalDuration = 0;

    const SpriteSheet* m_spriteSheet = nullptr;
};