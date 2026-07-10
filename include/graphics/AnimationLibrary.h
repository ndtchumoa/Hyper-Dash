#pragma once

#include "graphics/AnimationClip.h"
#include "resources/AnimationID.h"

#include <unordered_map>

class AnimationLibrary
{
public:

    AnimationLibrary()  = default;
    ~AnimationLibrary() = default;

    bool addClip(AnimationID id, const AnimationClip& clip);

    bool removeClip(AnimationID id);

    bool hasClip(AnimationID id) const;

    AnimationClip* getClip(AnimationID id);

    const AnimationClip* getClip(AnimationID id) const;

    void clear();

    std::size_t size() const;

    bool empty() const;

private:

    std::unordered_map<AnimationID, AnimationClip> m_clips;
};