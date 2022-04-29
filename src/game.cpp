#include "game.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "player.h"

Game::Game()
    : player(Vect<float>(50.0f, 50.0f))
{

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
    inputs();

    // Updating
    player.update(keys);

    Vect<int> renderOffset = player.getRenderOffset();

    // Rendering
    player.render(window, renderOffset);

    window.update();
}

void Game::loop()
{
    while (!quit) iteration();
}

void Game::inputs()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)quit = true;
        else handleKey(event.key.keysym.sym, event.type);
    }
}

void Game::handleKey(SDL_Keycode& key, Uint32& type)
{
    if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end())
        keys[key] = (type == SDL_KEYDOWN);
}