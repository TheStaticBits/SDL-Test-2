#include "game.h"

#include <iostream>
#include <unordered_map>
#include <cmath>
#include <chrono>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "player.h"
#include "vector.h"
#include "base.h"
#include "save.h"
#include "utility.h"
#include "shop.h"

Game::Game()
    : player(window), 
      base(window), shop(window),
      deltaTime(0.0f), lastTime(0), lastSaveTime(0)
{
    // Loading save if the person has one
    if (hasSave("save"))
    {
#ifdef __EMSCRIPTEN__
        char* save = getSave("save");
        readSave(std::string(save));
        free(save);
#else
        readSave(getSave("save"));
#endif
    }
}

Game::~Game()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "[Error] SDL failed to initialize: " << SDL_GetError() << std::endl;
        
        return false;
    }
    
    if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "[Error] SDL_image failed to initialize: " << IMG_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() == -1)
    {
        std::cout << "[Error] SDL_ttf failed to initialize: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void Game::iteration()
{
    calcDeltaTime();

    window.inputs();

    if (window.getResize())
    {
        shop.resize(window);
        player.resize(base, window);
    }

    // Updating
    Vect<int64_t> renderOffset = player.getRenderOffset();
    shop.update(window, deltaTime);
    if (!shop.isActive())
    {
        player.update(window, deltaTime, base);
        renderOffset = player.getRenderOffset();
        base.update(window, renderOffset);
    }

    // Rendering
    base.renderTiles(window, renderOffset);
    player.render(window, renderOffset);

    base.renderMenues(window, renderOffset);
    base.renderMinimap(window);
    
    shop.render(window);

    // Scale up and display frame
    window.update();

    // Saving every 5 seconds
    if (std::time(0) > lastSaveTime + SAVE_INTERVAL)
    {
        lastSaveTime = std::time(0);
        save();
    }

    // Save when pressing space or when you left click
    if (window.pKey(SDLK_SPACE) || window.button(SDL_BUTTON_LEFT)) save();
}

#ifdef __EMSCRIPTEN__
void it(void* void_game) 
{ 
    Game* game = (Game*)void_game; 
    game->iteration(); 
}
#endif

void Game::loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(it, this, 0, 1);
#else
    while (!window.isClosed()) iteration();
#endif
    save();
}

void Game::save()
{
    std::string save = "";
    save += player.getSave();
    save += " ~ ";
    save += base.getSave();

    setSave("save", save.c_str()); // For emscripten
}

void Game::readSave(const std::string save)
{
    std::vector<std::string> saveParts = util::split(save, " ~ ");

    for (std::string part : saveParts)
    {
        if (player.checkSavePart(part))
            player.readSave(part, window);
        else if (base.checkSavePart(part))
            base.readSave(part);
    }
}

void Game::calcDeltaTime()
{
    uint32_t currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
}