#define SDL_MAIN_HANDLED

#include "engine/Game.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

// Đi lên từ thư mục executable cho đến khi tìm thấy assets/
// Không hard-code số cấp thư mục — robust với mọi cấu hình build.
static bool setWorkingDirectoryToProjectRoot()
{
    char* basePath = SDL_GetBasePath();

    if (!basePath)
    {
        std::cerr << "[main] SDL_GetBasePath() failed.\n";
        return false;
    }

    std::string path(basePath);
    SDL_free(basePath);

    // Normalize: backslash → forward slash, bỏ trailing slash
    for (auto& c : path)
        if (c == '\\') c = '/';

    while (!path.empty() && path.back() == '/')
        path.pop_back();

    std::cout << "[main] Executable dir: " << path << '\n';

    // Đi lên tối đa 5 cấp để tìm thư mục chứa assets/
    for (int i = 0; i < 5; ++i)
    {
        const std::string testPath = path + "/assets";

#ifdef _WIN32
        const DWORD attr = GetFileAttributesA(testPath.c_str());
        const bool found = (attr != INVALID_FILE_ATTRIBUTES) &&
                           (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
        struct stat st;
        const bool found = (stat(testPath.c_str(), &st) == 0) &&
                           S_ISDIR(st.st_mode);
#endif

        if (found)
        {
#ifdef _WIN32
            SetCurrentDirectoryA(path.c_str());
#else
            chdir(path.c_str());
#endif
            std::cout << "[main] Working dir: " << path << '\n';
            return true;
        }

        // Lên một cấp
        const auto slash = path.rfind('/');
        if (slash == std::string::npos)
            break;

        path = path.substr(0, slash);
    }

    std::cerr << "[main] Cannot find assets/ directory.\n";
    return false;
}

int main()
{
    setWorkingDirectoryToProjectRoot();

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