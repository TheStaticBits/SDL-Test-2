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
      textSize(util::getSize(text)), 
      button(window, ButtonBlue, Vect<int64_t>(60, 80), "Buy", 10),
      active(false), l1PosX(WIN_WIDTH), l2PosX(WIN_WIDTH)
{
    
}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
}

void Shop::update(const Vect<int64_t>& mousePos,    
                  std::unordered_map<uint8_t, bool>& mouseButtons,
                  std::unordered_map<uint8_t, bool>& mouseHeldButtons)
{
    button.update(mousePos, mouseHeldButtons);
}

void Shop::render(Window& window)
{
    if (active)
    {
        Vect<int> textSizeInt = textSize.cast<int>();
        SDL_Rect dest = {50, 120, textSizeInt.x, textSizeInt.y};
        window.render(text, dest);
    }

    button.render(window, Vect<int64_t>{1, 0});
}