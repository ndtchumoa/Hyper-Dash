cmake_minimum_required(VERSION 3.20)

project(HyperDash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 via vcpkg
find_package(SDL2       CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf   CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

add_executable(HyperDash
    main.cpp

    # Engine
    src/engine/Game.cpp
    src/engine/Scene.cpp
    src/engine/SceneManager.cpp

    # Entities
    src/entities/Obstacle.cpp
    src/entities/ObstacleManager.cpp
    src/entities/Player.cpp

    # Graphics
    src/graphics/Animation.cpp
    src/graphics/AnimationClip.cpp
    src/graphics/AnimationController.cpp
    src/graphics/AnimationLibrary.cpp
    src/graphics/Animator.cpp
    src/graphics/Background.cpp
    src/graphics/Camera.cpp
    src/graphics/CharacterRenderer.cpp
    src/graphics/ParticleSystem.cpp
    src/graphics/SpriteSheet.cpp

    # Resources
    src/resources/ResourceLoader.cpp
    src/resources/ResourcePath.cpp

    # Scenes
    src/scenes/GameOverScene.cpp
    src/scenes/MenuScene.cpp
    src/scenes/PlayScene.cpp

    # Systems
    src/systems/AssetManager.cpp
    src/systems/AudioManager.cpp
    src/systems/CollisionSystem.cpp
    src/systems/InputSystem.cpp
    src/systems/SaveSystem.cpp

    # UI
    src/ui/Button.cpp
    src/ui/HUD.cpp
    src/ui/Label.cpp
    src/ui/ProgressBar.cpp
    src/ui/Score.cpp

    # World
    src/world/ParallaxLayer.cpp
    src/world/SpawnManager.cpp
    src/world/TileMap.cpp
    src/world/World.cpp
)

target_include_directories(HyperDash PRIVATE
    include
)

target_link_libraries(HyperDash PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)