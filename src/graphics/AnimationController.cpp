#include "graphics/AnimationController.h"

#include "graphics/Animator.h"

void AnimationController::setAnimator(Animator* animator)
{
    m_animator = animator;
}

void AnimationController::update(PlayerState state)
{
    if (!m_animator)
        return;

    // State không đổi → không cần play clip mới, TRỪ trường hợp:
    // clip hiện tại đã Finished (non-loop) mà state vẫn là Jump/Fall.
    // Trong trường hợp đó Animator đã dừng ở frame cuối — đúng.
    // Không cần restart.
    if (state == m_previousState)
        return;

    const AnimationID id     = resolveAnimationID(state);
    const bool        restart = (state == PlayerState::Jump);

    m_animator->play(id, restart);

    m_previousState = state;
}

AnimationID AnimationController::resolveAnimationID(
    PlayerState state) const
{
    switch (state)
    {
    case PlayerState::Idle: return AnimationID::PlayerIdle;
    case PlayerState::Run:  return AnimationID::PlayerRun;
    case PlayerState::Jump: return AnimationID::PlayerJump;
    case PlayerState::Fall: return AnimationID::PlayerFall;
    }

    return AnimationID::PlayerIdle;
}