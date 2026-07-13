#include "engine/SceneManager.h"
#include "engine/Scene.h"

void SceneManager::changeScene(std::unique_ptr<Scene> scene)
{
    if (m_currentScene)
        m_currentScene->clean();

    m_currentScene = std::move(scene);

    if (m_currentScene)
        m_currentScene->init();
}

void SceneManager::handleEvents(const SDL_Event& event)
{
    if (m_currentScene)
        m_currentScene->handleEvents(event);
}

void SceneManager::update(float deltaTime)
{
    if (m_currentScene)
        m_currentScene->update(deltaTime);
}

void SceneManager::render(SDL_Renderer* renderer)
{
    if (m_currentScene)
        m_currentScene->render(renderer);
}