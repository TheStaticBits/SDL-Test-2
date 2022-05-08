#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"

Game game;

#ifdef __EMSCRIPTEN__
void it() { game.iteration(); }
#endif

int main(int argc, char* args[]) 
{
    if (!game.initSDL())
        return -1;

    game.loop();
    game.save();
   
    return 0;
}