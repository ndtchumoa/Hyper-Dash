#pragma once

#include "graphics/AnimationFrame.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

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

    //-------------------------

    std::size_t getFrameCount() const;

    //-------------------------

    bool empty() const;

    //-------------------------

    void clear();

    //-------------------------

    void setLoop(bool loop);

    bool isLooping() const;

    //-------------------------

    std::uint32_t getTotalDuration() const;

private:

    void recalculateDuration();

private:

    std::string name;

    std::vector<AnimationFrame> frames;

    bool loop = true;

    std::uint32_t totalDuration = 0;

private:

    const SpriteSheet* spriteSheet = nullptr;

public:

    void setSpriteSheet(
        const SpriteSheet* sheet);

    const SpriteSheet*
    getSpriteSheet() const;
};