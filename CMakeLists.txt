cmake_minimum_required(VERSION 3.20)

project(HyperDash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

add_executable(HyperDash
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
)

target_include_directories(HyperDash PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(HyperDash PRIVATE
    SDL2::SDL2
)