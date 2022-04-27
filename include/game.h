#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

class Game
{
public:
    Game();
    ~Game();

    bool initSDL() const;

    void loop();

    void operator=(const Game&) = delete;

private:
    Window window;
    bool quit;

    SDL_Event event;
    
    void iteration();

    void inputs();
    void handleKey(SDL_Keycode& key, Uint32& type);
};