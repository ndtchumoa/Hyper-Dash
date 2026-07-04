#include "engine/SceneManager.h"
#include "engine/Scene.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>

void SceneManager::changeScene(std::unique_ptr<Scene> scene)
{
    if (currentScene)
    {
        currentScene->clean();
    }

    currentScene = std::move(scene);

    if (currentScene)
    {
        currentScene->init();
    }
}

void SceneManager::handleEvents(const SDL_Event& event)
{
    if (currentScene)
    {
        currentScene->handleEvents(event);
    }
}

void SceneManager::update()
{
    if (currentScene)
    {
        currentScene->update();
    }
}

void SceneManager::render(SDL_Renderer* renderer)
{
    if (currentScene)
    {
        currentScene->render(renderer);
    }
}