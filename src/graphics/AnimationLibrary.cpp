#include "graphics/AnimationLibrary.h"

bool AnimationLibrary::addClip(
    AnimationID id,
    const AnimationClip& clip)
{
    // Overwrite nếu id đã tồn tại — clip mới luôn thắng.
    m_clips.insert_or_assign(id, clip);
    return true;
}

bool AnimationLibrary::removeClip(AnimationID id)
{
    return m_clips.erase(id) > 0;
}

bool AnimationLibrary::hasClip(AnimationID id) const
{
    return m_clips.contains(id);
}

AnimationClip* AnimationLibrary::getClip(AnimationID id)
{
    auto it = m_clips.find(id);

    return (it != m_clips.end())
        ? &it->second
        : nullptr;
}

const AnimationClip* AnimationLibrary::getClip(
    AnimationID id) const
{
    auto it = m_clips.find(id);

    return (it != m_clips.end())
        ? &it->second
        : nullptr;
}

void AnimationLibrary::clear()
{
    m_clips.clear();
}

std::size_t AnimationLibrary::size() const
{
    return m_clips.size();
}

bool AnimationLibrary::empty() const
{
    return m_clips.empty();
}