#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "player.h"
#include "vector.h"
#include "base.h"
#include "shop.h"

class Game
{
public:
    Game();
    ~Game();

    void operator=(const Game&) = delete;

    static bool initSDL();

    void loop();
    
    void iteration();

    void save();
    void readSave(const std::string save);

private:
    Window window;
    Player player;
    Base base;
    Shop shop;

    float deltaTime;
    uint32_t lastTime;
    uint32_t lastSaveTime;
    static constexpr uint32_t SAVE_INTERVAL = 5; // Save every 5 seconds

    void calcDeltaTime();
};