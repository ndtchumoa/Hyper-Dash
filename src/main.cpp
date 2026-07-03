#define SDL_MAIN_HANDLED

#include "Game.h"
#include <iostream>

int main()
{
    Game game;

    if (!game.init())
    {
        std::cout << "Init failed!\n";
        std::cin.get();
        return 1;
    }

    game.run();
    return 0;
}