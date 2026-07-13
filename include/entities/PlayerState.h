#pragma once

// Tách PlayerState ra file riêng để:
// - AnimationController có thể include mà không kéo theo toàn bộ Player.h
// - Tránh circular dependency: graphics/ → entities/Player.h → graphics/
// - Dễ tái sử dụng nếu sau này có Enemy, NPC cùng state machine tương tự.

enum class PlayerState : unsigned char
{
    Idle,
    Run,
    Jump,
    Fall
};