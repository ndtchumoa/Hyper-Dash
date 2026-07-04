#pragma once

#include <memory>

#include <SDL2/SDL.h>

class Scene;

class SceneManager
{
public:
    void changeScene(std::unique_ptr<Scene> scene);

    void handleEvents(const SDL_Event& event);

    void update();

    void render(SDL_Renderer* renderer);

private:
    std::unique_ptr<Scene> currentScene;
};