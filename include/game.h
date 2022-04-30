#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "player.h"
#include "vector.h"
#include "base.h"

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
        SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_RETURN, SDLK_SPACE
    };

    std::unordered_map<SDL_Keycode, bool> keys;
    std::unordered_map<SDL_Keycode, bool> oneTimeKeys;
    std::unordered_map<Uint8, bool> mouseButtons;
    Vect<int> mousePos;

    Window window;
    Player player;
    Base base;

    bool quit;

    int lastTime;
    float deltaTime;
    
    void iteration();

    void calcDeltaTime();

    void inputs();
    void handleKey(SDL_Keycode& key, Uint32& type);
};