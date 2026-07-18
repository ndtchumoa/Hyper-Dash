#pragma once

#include "entities/Obstacle.h"
#include "entities/ObstacleCatalog.h"

#include <SDL2/SDL.h>

#include <vector>

class ResourceManager;

class ObstacleManager
{
public:

    bool init(
        ResourceManager& resources,
        int              groundY,
        int              screenWidth);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer) const;

    void reset();

    bool checkCollision(const SDL_Rect& playerBounds) const;

    bool checkPassed(int playerX);

    //--------------------------------------------------
    // Difficulty setters — gọi từ PlayScene mỗi frame
    //--------------------------------------------------

    void setSpeed(float pixelsPerSecond);

    void setSpawnInterval(float seconds);

    float getSpeed()         const;
    float getSpawnInterval() const;

private:

    void         spawn();
    ObstacleKind pickRandomKind() const;
    float        getRandomSpawnInterval() const;

private:

    std::vector<Obstacle> m_obstacles;

    // Không cache SDL_Texture* riêng nữa — mỗi kind có thể dùng
    // texture khác nhau (xem ObstacleCatalog). Tra cứu ResourceManager
    // mỗi lần spawn (O(1) array lookup, đã cache sẵn, không tốn kém).
    ResourceManager* m_resources = nullptr;

    int   m_groundY     = 0;
    int   m_screenWidth = 0;

    float m_spawnTimer    = 0.0f;
    float m_spawnInterval = 2.0f;
    float m_speed         = 360.0f;

    static constexpr float kMinSpawnInterval = 0.8f;
    static constexpr float kMaxSpawnInterval = 2.667f;
};