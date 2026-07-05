#include "graphics/AnimationClip.h"
#include "graphics/SpriteSheet.h"

#include <stdexcept>

AnimationClip::AnimationClip(std::string clipName)
    : name(std::move(clipName))
{
}

void AnimationClip::setName(const std::string& newName)
{
    name = newName;
}

const std::string& AnimationClip::getName() const
{
    return name;
}

void AnimationClip::addFrame(const AnimationFrame& frame)
{
    frames.push_back(frame);

    totalDuration += frame.duration;
}

const AnimationFrame&
AnimationClip::getFrame(std::size_t index) const
{
    if (index >= frames.size())
    {
        throw std::out_of_range(
            "AnimationClip::getFrame");
    }

    return frames[index];
}

AnimationFrame&
AnimationClip::getFrame(std::size_t index)
{
    if (index >= frames.size())
    {
        throw std::out_of_range(
            "AnimationClip::getFrame");
    }

    return frames[index];
}

std::size_t
AnimationClip::getFrameCount() const
{
    return frames.size();
}

bool AnimationClip::empty() const
{
    return frames.empty();
}

void AnimationClip::clear()
{
    frames.clear();

    totalDuration = 0;
}

void AnimationClip::setLoop(bool value)
{
    loop = value;
}

bool AnimationClip::isLooping() const
{
    return loop;
}

std::uint32_t
AnimationClip::getTotalDuration() const
{
    return totalDuration;
}

void AnimationClip::recalculateDuration()
{
    totalDuration = 0;

    for (const auto& frame : frames)
    {
        totalDuration += frame.duration;
    }
}

void AnimationClip::setSpriteSheet(
    const SpriteSheet* sheet)
{
    spriteSheet = sheet;
}

const SpriteSheet*
AnimationClip::getSpriteSheet() const
{
    return spriteSheet;
}