#include "shop.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"
#include "button.h"

Shop::Shop(Window& window)
    : text(window.getTextImg(window.font(16), "Shop", {255, 255, 255})),
      textSize(util::getSize(text)), active(false),
      l1PosX(WIN_WIDTH), l2PosX(WIN_WIDTH)
{
    
}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
}

void Shop::update(const Vect<int64_t>& mousePos,    
                  const std::unordered_map<uint8_t, bool>& mouseButtons)
{

}

void Shop::render(Window& window)
{
    if (active)
    {
        Vect<int> textSizeInt = textSize.cast<int>();
        SDL_Rect dest = {50, 120, textSizeInt.x, textSizeInt.y};
        window.render(text, dest);
    }
}