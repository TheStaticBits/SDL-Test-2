#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

class Shop
{
public:
    Shop(Window& window);
    ~Shop();

    void operator=(const Shop&) = delete;

    void update();
    void render(Window& window);

private:
    SDL_Texture* text;
    Vect<int> textSize;
};