#pragma once

#include <SDL2/SDL.h>

#include <memory>

class Scene;

class SceneManager
{
public:

    void changeScene(std::unique_ptr<Scene> scene);

    void handleEvents(const SDL_Event& event);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer);

private:

    std::unique_ptr<Scene> m_currentScene;
};