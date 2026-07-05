#include "scenes/PlayScene.h"

#include "engine/Game.h"

#include <SDL2/SDL.h>

#include <iostream>

PlayScene::PlayScene(Game& game)
    : Scene(game)
{
}

void PlayScene::init()
{
    ground = { 0, 720 - GROUND_HEIGHT, 1280, GROUND_HEIGHT };

    background.init(game.getAssets());

    player.init(game.getAssets(), ground.y);

    obstacleManager.init(
        game.getAssets(),
        ground.y,
        ground.w);

    score.init(game.getRenderer());
    score.reset();

    std::cout << "[PlayScene] Initialized.\n";
}

void PlayScene::handleEvents(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        game.quit();
        break;

    case SDL_KEYDOWN:

        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            game.quit();
            break;

        case SDLK_SPACE:
            player.jump();
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void PlayScene::update(float deltaTime)
{
    background.update();

    player.update(deltaTime);

    obstacleManager.update();

    //==============================
    // Collision
    //==============================

    if (obstacleManager.checkCollision(player.getBounds()))
    {
        std::cout << "[PlayScene] Game Over.\n";

        // TODO: chuyển sang GameOverScene trong sprint sau
        // game.getSceneManager().changeScene(
        //     std::make_unique<GameOverScene>(game));
    }

    //==============================
    // Score
    //==============================

    if (obstacleManager.checkPassed(player.getBounds().x))
    {
        score.addPoint();
    }
}

void PlayScene::render(SDL_Renderer* renderer)
{
    background.render(renderer);

    player.render(renderer);

    obstacleManager.render(renderer);

    // score.render(renderer);  // TODO: re-enable khi Score được migrate sang TTF

    // Ground placeholder
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &ground);
}

void PlayScene::clean()
{
    background.clean();

    score.clean();

    std::cout << "[PlayScene] Cleaned.\n";
}