#include "systems/DifficultyManager.h"

#include <algorithm>
#include <cmath>

void DifficultyManager::reset()
{
    m_state = DifficultyState{};
}

void DifficultyManager::update(float elapsed, int score)
{
    //----------------------------------------------------------
    // 1. Level — dựa trên score (discrete)
    //----------------------------------------------------------

    const int level = 1 + (score / kLevelScoreThreshold);

    m_state.level = level;

    //----------------------------------------------------------
    // 2. Obstacle speed — linear ramp theo thời gian
    //    + bonus nhân per-level
    //----------------------------------------------------------

    // Tốc độ cơ bản tăng dần theo thời gian
    const float rampedSpeed =
        kBaseObstacleSpeed + kSpeedRampPerSecond * elapsed;

    // Level bonus: mỗi level thêm 5% so với rampedSpeed
    const float levelMultiplier =
        1.0f + kLevelSpeedBonus * static_cast<float>(level - 1);

    m_state.obstacleSpeed = std::min(
        rampedSpeed * levelMultiplier,
        kMaxObstacleSpeed);

    //----------------------------------------------------------
    // 3. Spawn interval — giảm dần theo thời gian
    //----------------------------------------------------------

    m_state.spawnInterval = std::max(
        kBaseSpawnInterval - kSpawnRampPerSecond * elapsed,
        kMinSpawnInterval);

    //----------------------------------------------------------
    // 4. Background scroll multiplier
    //    Tỉ lệ thuận với speed để background cảm giác đồng bộ.
    //----------------------------------------------------------

    m_state.bgScrollMultiplier =
        m_state.obstacleSpeed / kBaseObstacleSpeed;
}

const DifficultyState& DifficultyManager::getState() const
{
    return m_state;
}

int DifficultyManager::getLevel() const
{
    return m_state.level;
}