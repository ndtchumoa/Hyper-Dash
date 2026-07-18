#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "engine/SceneManager.h"
#include "resources/ResourceManager.h"
#include "systems/AudioManager.h"

#include <cstdint>

class Game
{
public:

    Game();
    ~Game();

    bool init();
    void run();
    void clean();

    //==============================
    // Getters
    //==============================

    SDL_Renderer*    getRenderer()      const { return m_renderer;      }
    SDL_Window*      getWindow()        const { return m_window;        }
    ResourceManager& getResources()           { return m_resources;     }
    SceneManager&    getSceneManager()        { return m_sceneManager;  }
    AudioManager&    getAudio()                { return m_audio;        }
    bool             isRunning()        const { return m_running;       }

    void quit() { m_running = false; }

private:

    void handleEvents();
    void update(float deltaTime);
    void render();

private:

    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;

    bool          m_running   = false;
    bool          m_audioReady = false;
    std::uint32_t m_lastTicks = 0;

    ResourceManager m_resources;
    SceneManager    m_sceneManager;
    AudioManager    m_audio;

    static constexpr int          kWindowWidth  = 1280;
    static constexpr int          kWindowHeight = 720;
    static constexpr std::uint32_t kMaxDeltaMs  = 100;
};