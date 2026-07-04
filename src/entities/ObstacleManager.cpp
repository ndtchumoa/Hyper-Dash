#include "entities/ObstacleManager.h"

#include "entities/Obstacle.h"
#include "systems/AssetManager.h"

#include <algorithm>
#include <cstdlib>

bool ObstacleManager::init(
    AssetManager& assets,
    int ground,
    int screenW)
{
    groundY = ground;

    screenWidth = screenW;

    obstacleTexture =
        assets.getTexture(
            "assets/textures/obstacle/Ores.png"
        );

    reset();

    return obstacleTexture != nullptr;
}

void ObstacleManager::reset()
{
    obstacles.clear();

    spawnTimer = 0;

    spawnInterval = getRandomSpawnTime();

    speed = 6.0f;
}

void ObstacleManager::spawn()
{
    obstacles.emplace_back(
        obstacleTexture,
        screenWidth,
        groundY,
        speed
    );
}

void ObstacleManager::update()
{
    spawnTimer++;

    if (spawnTimer >= spawnInterval)
    {
        spawn();

        spawnTimer = 0;

        spawnInterval = getRandomSpawnTime();
    }

    for (auto& obstacle : obstacles)
    {
        obstacle.update();
    }

    obstacles.erase(
        std::remove_if(
            obstacles.begin(),
            obstacles.end(),
            [](const Obstacle& obstacle)
            {
                return obstacle.isOffScreen();
            }),
        obstacles.end());
}

void ObstacleManager::render(SDL_Renderer* renderer)
{
    for (auto& obstacle : obstacles)
    {
        obstacle.render(renderer);
    }
}

bool ObstacleManager::checkCollision(
    const SDL_Rect& playerBounds)
{
    for (auto& obstacle : obstacles)
    {
        SDL_Rect obstacleBounds =
            obstacle.getBounds();

        if (SDL_HasIntersection(
                &playerBounds,
                &obstacleBounds))
        {
            return true;
        }
    }

    return false;
}

bool ObstacleManager::checkPassed(int playerX)
{
    bool scored = false;

    for (auto& obstacle : obstacles)
    {
        if (obstacle.hasPassedPlayer(playerX))
        {
            scored = true;
        }
    }

    return scored;
}

int ObstacleManager::getRandomSpawnTime() const
{
    return 90 + std::rand() % 70;
}