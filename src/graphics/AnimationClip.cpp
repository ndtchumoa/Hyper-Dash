#include "graphics/AnimationClip.h"
#include "graphics/SpriteSheet.h"

#include <stdexcept>

AnimationClip::AnimationClip(std::string name)
    : m_name(std::move(name))
{
}

void AnimationClip::setName(const std::string& name)
{
    m_name = name;
}

const std::string& AnimationClip::getName() const
{
    return m_name;
}

void AnimationClip::addFrame(const AnimationFrame& frame)
{
    m_frames.push_back(frame);
    m_totalDuration += frame.duration;
}

const AnimationFrame& AnimationClip::getFrame(
    std::size_t index) const
{
    if (index >= m_frames.size())
        throw std::out_of_range("AnimationClip::getFrame");

    return m_frames[index];
}

AnimationFrame& AnimationClip::getFrame(
    std::size_t index)
{
    if (index >= m_frames.size())
        throw std::out_of_range("AnimationClip::getFrame");

    return m_frames[index];
}

std::size_t AnimationClip::getFrameCount() const
{
    return m_frames.size();
}

bool AnimationClip::empty() const
{
    return m_frames.empty();
}

void AnimationClip::clear()
{
    m_frames.clear();
    m_totalDuration = 0;
}

void AnimationClip::setLoop(bool loop)
{
    m_loop = loop;
}

bool AnimationClip::isLooping() const
{
    return m_loop;
}

std::uint32_t AnimationClip::getTotalDuration() const
{
    return m_totalDuration;
}

void AnimationClip::setSpriteSheet(
    const SpriteSheet* sheet)
{
    m_spriteSheet = sheet;
}

const SpriteSheet* AnimationClip::getSpriteSheet() const
{
    return m_spriteSheet;
}