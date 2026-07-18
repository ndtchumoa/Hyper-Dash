#include "entities/ObstacleManager.h"

#include "resources/ResourceManager.h"

#include <algorithm>
#include <cstdlib>
#include <random>

bool ObstacleManager::init(
    ResourceManager& resources,
    int              groundY,
    int              screenWidth)
{
    m_groundY     = groundY;
    m_screenWidth = screenWidth;
    m_resources   = &resources;

    reset();

    return true;
}

void ObstacleManager::reset()
{
    m_obstacles.clear();
    m_spawnTimer    = 0.0f;
    m_spawnInterval = kMaxSpawnInterval;
    m_speed         = 360.0f;
}

void ObstacleManager::setSpeed(float pixelsPerSecond)
{
    m_speed = pixelsPerSecond;
}

void ObstacleManager::setSpawnInterval(float seconds)
{
    // Clamp trong ObstacleManager để tránh giá trị phi lý
    // dù DifficultyManager đã clamp ở phía nó.
    m_spawnInterval =
        std::clamp(seconds, kMinSpawnInterval, kMaxSpawnInterval);
}

float ObstacleManager::getSpeed() const
{
    return m_speed;
}

float ObstacleManager::getSpawnInterval() const
{
    return m_spawnInterval;
}

void ObstacleManager::spawn()
{
    const ObstacleKind kind = pickRandomKind();
    const ObstacleDef  def  = ObstacleCatalog::resolve(kind);

    SDL_Texture* texture =
        m_resources ? m_resources->get(def.texture) : nullptr;

    m_obstacles.emplace_back(
        texture,
        def.sourceRect,
        def.renderWidth,
        def.renderHeight,
        m_screenWidth,
        m_groundY,
        m_speed);
}

ObstacleKind ObstacleManager::pickRandomKind() const
{
    // Random đều trên tất cả kind (Ore + 6 Garden) — quyết định
    // "spawn cái gì" thuộc về ObstacleManager, đúng nơi đã sở hữu
    // toàn bộ logic spawn (spawn timing, spawn position).
    static std::mt19937 rng{ std::random_device{}() };

    std::uniform_int_distribution<int> dist(
        0, ObstacleCatalog::count() - 1);

    return static_cast<ObstacleKind>(dist(rng));
}

void ObstacleManager::update(float deltaTime)
{
    m_spawnTimer += deltaTime;

    if (m_spawnTimer >= m_spawnInterval)
    {
        spawn();
        m_spawnTimer = 0.0f;
        // Không reset spawnInterval — DifficultyManager điều khiển
    }

    for (auto& obstacle : m_obstacles)
        obstacle.update(deltaTime);

    std::erase_if(
        m_obstacles,
        [](const Obstacle& o) { return o.isOffScreen(); });
}

void ObstacleManager::render(SDL_Renderer* renderer) const
{
    for (const auto& obstacle : m_obstacles)
        obstacle.render(renderer);
}

bool ObstacleManager::checkCollision(
    const SDL_Rect& playerBounds) const
{
    for (const auto& obstacle : m_obstacles)
    {
        SDL_Rect bounds = obstacle.getBounds();

        if (SDL_HasIntersection(&playerBounds, &bounds))
            return true;
    }

    return false;
}

bool ObstacleManager::checkPassed(int playerX)
{
    bool scored = false;

    for (auto& obstacle : m_obstacles)
    {
        if (obstacle.hasPassedPlayer(playerX))
            scored = true;
    }

    return scored;
}

float ObstacleManager::getRandomSpawnInterval() const
{
    const float range = kMaxSpawnInterval - kMinSpawnInterval;
    const float r     =
        static_cast<float>(std::rand()) /
        static_cast<float>(RAND_MAX);

    return kMinSpawnInterval + r * range;
}