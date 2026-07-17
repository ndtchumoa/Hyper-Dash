#pragma once

// DifficultyState là snapshot của độ khó tại một thời điểm.
// PlayScene đọc state này và truyền xuống các systems tương ứng.
// Không có raw pointer, không có dependency — dễ serialize sau này.
struct DifficultyState
{
    float obstacleSpeed      = 360.0f;  // pixels/giây
    float spawnInterval      = 2.0f;    // giây
    float bgScrollMultiplier = 1.0f;    // nhân với base speed từng layer
    int   level              = 1;       // hiện tại ở level nào
};

// DifficultyManager có đúng một trách nhiệm:
// Nhận thời gian chơi và score → trả về DifficultyState.
//
// Không biết gì về ObstacleManager, Background hay Player.
// Không giữ reference đến bất kỳ gameplay system nào.
// Chỉ là pure calculation — dễ test, dễ tune.
//
// Scaling model:
//   - Tốc độ tăng liên tục theo thời gian (linear ramp).
//   - Mỗi kLevelScoreThreshold điểm → lên một level (discrete jump).
//   - Tốc độ bị cap ở kMaxObstacleSpeed để game vẫn playable.
class DifficultyManager
{
public:

    DifficultyManager() = default;

    void reset();

    // Gọi mỗi frame từ PlayScene.
    // elapsed: tổng thời gian đã chơi (giây).
    // score: score hiện tại.
    void update(float elapsed, int score);

    const DifficultyState& getState() const;

    int getLevel() const;

private:

    DifficultyState m_state;

    // ── Tuning constants ──────────────────────────────────────
    //
    // Tốc độ ban đầu (pixels/s)
    static constexpr float kBaseObstacleSpeed    = 520.0f;

    // Tăng bao nhiêu pixels/s mỗi giây chơi (linear)
    static constexpr float kSpeedRampPerSecond   = 3.0f;

    // Tốc độ tối đa (pixels/s)
    static constexpr float kMaxObstacleSpeed     = 800.0f;

    // Spawn interval ban đầu (giây)
    static constexpr float kBaseSpawnInterval    = 2.0f;

    // Spawn interval tối thiểu (giây) — không để dưới này
    static constexpr float kMinSpawnInterval     = 0.8f;

    // Mỗi giây, spawn interval giảm bao nhiêu (giây)
    static constexpr float kSpawnRampPerSecond   = 0.015f;

    // Số điểm để lên 1 level
    static constexpr int   kLevelScoreThreshold  = 10;

    // Mỗi level, speed tăng thêm bao nhiêu % (nhân thêm vào sau ramp)
    static constexpr float kLevelSpeedBonus      = 0.03f;  // 5%/level

    // Background scroll multiplier tăng theo speed ratio
    // bgScrollMultiplier = currentSpeed / baseSpeed
    // Giúp background cảm giác đồng bộ với obstacle speed.
};