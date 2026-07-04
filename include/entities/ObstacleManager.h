#pragma once

#include <SDL2/SDL.h>

#include <vector>

class AssetManager;
class Obstacle;

class ObstacleManager
{
public:
    bool init(
        AssetManager& assets,
        int groundY,
        int screenWidth
    );

    void update();

    void render(SDL_Renderer* renderer);

    void reset();

    bool checkCollision(const SDL_Rect& playerBounds);

    bool checkPassed(int playerX);

private:
    void spawn();

    int getRandomSpawnTime() const;

private:
    std::vector<Obstacle> obstacles;

    SDL_Texture* obstacleTexture = nullptr;

    int groundY = 0;

    int screenWidth = 0;

    int spawnTimer = 0;

    int spawnInterval = 120;

    float speed = 6.0f;
};