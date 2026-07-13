cmake_minimum_required(VERSION 3.20)

project(HyperDash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2       CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf   CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

add_executable(HyperDash
    main.cpp

    # ── Engine ────────────────────────────────────────────────
    src/engine/Game.cpp
    src/engine/SceneManager.cpp
    # src/engine/Scene.cpp       — header-only base class, no impl needed
    # src/engine/Config.cpp      — future: game config system
    # src/engine/Entity.cpp      — future: base entity class

    # ── Entities ──────────────────────────────────────────────
    src/entities/Obstacle.cpp
    src/entities/ObstacleManager.cpp
    src/entities/Player.cpp
    # src/entities/Character.cpp — future: shared character base
    # src/entities/Enemy.cpp     — future: enemy types
    # src/entities/Pickup.cpp    — future: collectible items

    # ── Graphics ──────────────────────────────────────────────
    src/graphics/AnimationClip.cpp
    src/graphics/AnimationController.cpp
    src/graphics/AnimationLibrary.cpp
    src/graphics/Animator.cpp
    src/graphics/Background.cpp
    src/graphics/CharacterRenderer.cpp
    src/graphics/SpriteSheet.cpp
    # src/graphics/Camera.cpp        — future: camera system
    # src/graphics/ParticleSystem.cpp — future: particle effects
    # NOTE: src/graphics/Animation.cpp đã bị xóa (hệ thống cũ)

    # ── Resources ─────────────────────────────────────────────
    src/resources/ResourceLoader.cpp
    src/resources/ResourcePath.cpp
    src/resources/ResourceManager.cpp

    # ── Scenes ────────────────────────────────────────────────
    src/scenes/GameOverScene.cpp
    src/scenes/MenuScene.cpp
    src/scenes/PlayScene.cpp

    # ── Systems ───────────────────────────────────────────────
    src/systems/DifficultyManager.cpp
    # src/systems/AudioManager.cpp    — future: sound/music
    # src/systems/CollisionSystem.cpp — future: physics collision
    # src/systems/InputSystem.cpp     — future: input mapping
    # src/systems/SaveSystem.cpp      — future: save/load
    # NOTE: src/systems/AssetManager.cpp đã bị xóa (replaced by ResourceManager)
    # NOTE: src/systems/Animation.cpp và AnimationManager.cpp đã bị xóa

    # ── UI ────────────────────────────────────────────────────
    src/ui/Button.cpp
    src/ui/Label.cpp
    src/ui/Score.cpp
    # src/ui/HUD.cpp         — future: heads-up display
    # src/ui/ProgressBar.cpp — future: health/stamina bar

    # ── World ─────────────────────────────────────────────────
    # src/world/ParallaxLayer.cpp — future: advanced parallax
    # src/world/SpawnManager.cpp  — future: entity spawning
    # src/world/TileMap.cpp       — future: tile-based levels
    # src/world/World.cpp         — future: world container
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