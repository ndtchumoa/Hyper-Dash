#include "graphics/AnimationLibrary.h"

bool AnimationLibrary::addClip(
    AnimationID id,
    const AnimationClip& clip)
{
    auto result = clips.emplace(id, clip);

    if (!result.second)
    {
        result.first->second = clip;
    }

    return true;
}

bool AnimationLibrary::removeClip(
    AnimationID id)
{
    return clips.erase(id) > 0;
}

bool AnimationLibrary::hasClip(
    AnimationID id) const
{
    return clips.find(id) != clips.end();
}

AnimationClip*
AnimationLibrary::getClip(
    AnimationID id)
{
    auto it = clips.find(id);

    if (it == clips.end())
    {
        return nullptr;
    }

    return &it->second;
}

const AnimationClip*
AnimationLibrary::getClip(
    AnimationID id) const
{
    auto it = clips.find(id);

    if (it == clips.end())
    {
        return nullptr;
    }

    return &it->second;
}

void AnimationLibrary::clear()
{
    clips.clear();
}

std::size_t
AnimationLibrary::size() const
{
    return clips.size();
}

bool
AnimationLibrary::empty() const
{
    return clips.empty();
}