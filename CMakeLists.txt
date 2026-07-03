cmake_minimum_required(VERSION 3.20)

project(HyperDash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(HyperDash
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Obstacle.cpp
)

target_include_directories(HyperDash PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_directories(HyperDash PRIVATE
    ${SDL2_LIBRARY_DIRS}
)

target_link_libraries(HyperDash PRIVATE
    ${SDL2_LIBRARIES}
)