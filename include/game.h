#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "player.h"

class Game
{
public:
    Game();
    ~Game();

    bool initSDL() const;

    void loop();

    void operator=(const Game&) = delete;

private:
    inline static const std::vector<SDL_Keycode> allowedKeys = {
        SDLK_w, SDLK_s, SDLK_a, SDLK_d,
    };

    std::unordered_map<SDL_Keycode, bool> keys;

    Window window;
    bool quit;

    Player player;

    SDL_Event event;
    
    void iteration();

    void inputs();
    void handleKey(SDL_Keycode& key, Uint32& type);
};