#pragma once

#include <SDL2/SDL.h>

class Game;

class Scene
{
public:
    explicit Scene(Game& game)
        : game(game)
    {
    }

    virtual ~Scene() = default;

    virtual void init() = 0;

    virtual void handleEvents(const SDL_Event& event) = 0;

    virtual void update() = 0;

    virtual void render(SDL_Renderer* renderer) = 0;

    virtual void clean() = 0;

protected:
    Game& game;
};