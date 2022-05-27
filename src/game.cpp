#include "game.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>
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
    : mousePos{0, 0}, 
      player(Vect<float>(WIN_WIDTH / 2, WIN_HEIGHT)), 
      base(window), shop(window),
      quit(false), deltaTime(0.0f), 
      lastTime(0), lastSaveTime(0)
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
    inputs();

    // Updating
    Vect<int64_t> renderOffset = player.getRenderOffset();
    shop.update(mousePos, mouseButtons, mouseHeldButtons, deltaTime);
    if (!shop.isActive())
    {
        player.update(keys, base, deltaTime);
        renderOffset = player.getRenderOffset();
        base.update(keys, mouseButtons, mouseHeldButtons, mousePos, renderOffset);
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
    if (keys[SDLK_SPACE] || mouseButtons[SDL_BUTTON_LEFT]) save();
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
    while (!quit) iteration();
    save();
#endif
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
            player.readSave(part);
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
                mouseButtons[event.button.button] = true; 
                mouseHeldButtons[event.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseHeldButtons[event.button.button] = false; break;
            
            default:
                handleKey(event.key.keysym.sym, event.type); break;
        }
    }

    Vect<int> mouseRetrieve;
    SDL_GetMouseState(&mouseRetrieve.x, &mouseRetrieve.y);
    // Adjusting based on the scale of the screen
    mousePos = mouseRetrieve.cast<int64_t>() / WIN_SCALE;
}

void Game::handleKey(SDL_Keycode& key, Uint32& type)
{
    if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end())
        keys[key] = (type == SDL_KEYDOWN);
}