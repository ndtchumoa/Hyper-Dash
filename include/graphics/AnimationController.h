#pragma once

#include "resources/AnimationID.h"
#include "entities/PlayerState.h"

class Animator;

// AnimationController có đúng một trách nhiệm:
// Đọc PlayerState từ gameplay và quyết định AnimationID nào
// cần được play trên Animator.
//
// Không biết gì về physics, input hay SDL.
// Không biết gì về texture hay rendering.
//
// Mở rộng trong tương lai: AnimationController có thể xử lý
// blending, transition delay, priority queue, v.v.
// mà không ảnh hưởng đến Player hay Animator.
class AnimationController
{
public:

    AnimationController() = default;

    // Gắn Animator mà controller này sẽ điều khiển.
    void setAnimator(Animator* animator);

    // Gọi mỗi frame từ Player sau khi state đã được cập nhật.
    // Ánh xạ state → AnimationID và gọi animator.play() nếu cần.
    void update(PlayerState state);

private:

    AnimationID resolveAnimationID(PlayerState state) const;

private:

    Animator*   m_animator     = nullptr;
    PlayerState m_previousState = PlayerState::Idle;
};