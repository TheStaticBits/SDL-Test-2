#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"

int main(int argc, char* args[]) 
{
    Game game;

    if (!game.initSDL())
        return -1;
    
    game.loop();
    
    return 0;
}