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
    //==========================
    // Ground
    //==========================

    ground =
    {
        0,
        720 - GROUND_HEIGHT,
        1280,
        GROUND_HEIGHT
    };

    //==========================
    // Background
    //==========================

    background.init(game.getAssets());

    //==========================
    // Player
    //==========================

    player.init(
        game.getAssets(),
        ground.y
    );

    //==========================
    // Obstacles
    //==========================

    obstacleManager.init(
        game.getAssets(),
        ground.y,
        ground.w
    );

    //==========================
    // Score
    //==========================

    score.init(
        game.getRenderer()
    );

    score.reset();

    std::cout << "PlayScene initialized\n";
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

void PlayScene::update()
{
    background.update();

    player.update();

    obstacleManager.update();

    //==========================
    // Collision
    //==========================

    if (obstacleManager.checkCollision(player.getBounds()))
    {
        std::cout << "Game Over\n";

        // Sprint sau:
        // game.getSceneManager().changeScene(
        //     std::make_unique<GameOverScene>(game));
    }

    //==========================
    // Score
    //==========================

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

    // score.render(renderer);

    SDL_SetRenderDrawColor(
        renderer,
        80,
        80,
        80,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &ground
    );
}

void PlayScene::clean()
{
    background.clean();

    score.clean();

    std::cout << "PlayScene cleaned\n";
}