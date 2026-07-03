#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Hyper Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cout << "Window Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer)
    {
        std::cout << "Renderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //========================
    // Player
    //========================

    SDL_Rect player = {100, 500, 50, 50};

    float velocityY = 0.0f;

    bool isJumping = false;

    const float gravity = 0.6f;
    const float jumpPower = -12.0f;

    //========================

    bool running = true;
    SDL_Event event;

    while (running)
    {
        //----------------------
        // Event
        //----------------------

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        //----------------------
        // Input
        //----------------------

        const Uint8* keyboard = SDL_GetKeyboardState(nullptr);

        if (keyboard[SDL_SCANCODE_SPACE] && !isJumping)
        {
            velocityY = jumpPower;
            isJumping = true;
        }

        //----------------------
        // Physics
        //----------------------

        velocityY += gravity;
        player.y += static_cast<int>(velocityY);

        if (player.y >= 500)
        {
            player.y = 500;
            velocityY = 0;
            isJumping = false;
        }

        //----------------------
        // Render
        //----------------------

        // Background
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Ground
        SDL_Rect ground = {0, 550, WINDOW_WIDTH, 50};
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &ground);

        // Player
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}