#pragma once

#include "engine/Scene.h"

#include "graphics/Background.h"
#include "entities/Player.h"
#include "entities/ObstacleManager.h"
#include "ui/Score.h"

class PlayScene : public Scene
{
public:

    explicit PlayScene(Game& game);

    void init()                               override;
    void handleEvents(const SDL_Event& event) override;
    void update(float deltaTime)              override;
    void render(SDL_Renderer* renderer)       override;
    void clean()                              override;

private:

    Background      background;
    Player          player;
    ObstacleManager obstacleManager;
    Score           score;

    SDL_Rect ground{};

    static constexpr int GROUND_HEIGHT = 80;
};