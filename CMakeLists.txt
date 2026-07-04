cmake_minimum_required(VERSION 3.20)

project(HyperDash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm các thư viện SDL2 thông qua vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

# Liệt kê toàn bộ source files dựa theo tree
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
    
    # Scenes
    src/scenes/GameOverScene.cpp
    src/scenes/MenuScene.cpp
    src/scenes/PlayScene.cpp
    
    # Systems
    src/systems/Animation.cpp
    src/systems/AnimationManager.cpp
    src/systems/AssetManager.cpp
    src/systems/AudioManager.cpp
    src/systems/InputSystem.cpp
    
    # UI
    src/ui/Score.cpp
    
    # World
    src/world/Background.cpp
)

# Chỉ định thư mục chứa các file header (.h)
target_include_directories(HyperDash PRIVATE
    include
)

# Link project với các thư viện SDL2
target_link_libraries(HyperDash PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)