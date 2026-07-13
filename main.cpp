#define SDL_MAIN_HANDLED

#include "engine/Game.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    // Seed rand() cho ObstacleManager spawn interval.
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Game game;

    if (!game.init())
    {
        std::cerr << "[main] Game init failed.\n";
        return 1;
    }

    game.run();

    return 0;
}