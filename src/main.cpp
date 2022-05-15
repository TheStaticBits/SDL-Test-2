#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"

int main(int argc, char* args[]) 
{
    if (!Game::initSDL())
        return -1;
    
    Game game;

#ifdef __EMSCRIPTEN__
void it() { game.iteration(); }
#endif

    game.loop();
    game.save();
   
    return 0;
}