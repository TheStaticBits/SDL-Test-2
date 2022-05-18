#include "shop.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

Shop::Shop(Window& window)
    : text(window.getTextImg(window.font(16), "Shop", {255, 255, 255})),
      textSize(util::getSize(text)), active(false)
{

}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
}

void Shop::update()
{

}

void Shop::render(Window& window)
{
    if (active)
    {
        SDL_Rect dest = {50, 120, textSize.x, textSize.y};
        window.render(text, dest);
    }
}