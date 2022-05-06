#include "game.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cmath>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "player.h"
#include "vector.h"
#include "base.h"
#include "save.h"

Game::Game()
    : mousePos{0, 0}, player(Vect<float>(WIN_WIDTH / 2, WIN_HEIGHT)), quit(false), lastTime(0), deltaTime(0.0f)
{
    setSave("save", "yoylecake");
    char* saveCharP = getSave("save");
    std::string save = saveCharP;
// Emscripten C strings compatability
#ifdef __EMSCRIPTEN__
    free(saveCharP);
#endif

    std::cout << save << std::endl;
}

Game::~Game()
{
    IMG_Quit();
    SDL_Quit();
}

bool Game::initSDL() const
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || !IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "[Error] SDL failed to initialize: " << SDL_GetError() << std::endl;
        
        return false;
    }

    return true;
}

void Game::iteration()
{
    calcDeltaTime();
    inputs();

    // Updating
    player.update(keys, base, deltaTime);
    Vect<int> renderOffset = player.getRenderOffset();
    base.update(keys, mouseButtons, mousePos, renderOffset);

    // Rendering
    base.render(window, renderOffset);
    player.render(window, renderOffset);

    window.update();
}

#ifdef __EMSCRIPTEN__
void it(); // Calls Game::iteration() in main.cpp
#endif

void Game::loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(it, 0, 1);
#else
    while (!quit) iteration();
#endif
}

void Game::calcDeltaTime()
{
    int currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
}

void Game::inputs()
{
    // Resetting
    mouseButtons.clear();

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true; break;
            
            case SDL_MOUSEBUTTONDOWN:
                mouseButtons[event.button.button] = true; break;
            
            default:
                handleKey(event.key.keysym.sym, event.type); break;
        }
    }

    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    // Adjusting based on the scale of the screen
    mousePos.x = (int)floor(mousePos.x / WIN_SCALE);
    mousePos.y = (int)floor(mousePos.y / WIN_SCALE);
}

void Game::handleKey(SDL_Keycode& key, Uint32& type)
{
    if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end())
    {
        keys[key] = (type == SDL_KEYDOWN);
    }
}