#include "shop.h"

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"
#include "button.h"

Shop::Shop(Window& window)
    : text(window.getTextImg(window.font(10), "Shop", {255, 255, 255})),
      textSize(util::getSize(text)), 
      l1Size(Vect<uint32_t>(80, WIN_HEIGHT)),
      BShop(window, ButtonBlue, Vect<int64_t>(60, 80)),
      active(false), l1PosX(WIN_WIDTH), l2PosX(WIN_WIDTH)
{
    
}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
}

void Shop::update(const Vect<int64_t>& mousePos,    
                  std::unordered_map<uint8_t, bool>& mouseButtons,
                  std::unordered_map<uint8_t, bool>& mouseHeldButtons,
                  float deltaTime)
{
    BShop.update(mousePos, mouseHeldButtons);
    if (BShop.isActivated()) active = !active;

    // Shop sliding graphics
    int64_t moveTo;

    if (active) 
        moveTo = static_cast<int64_t>(WIN_WIDTH - l1Size.x);
    else
        moveTo = WIN_WIDTH;
    
    l1PosX -= (l1PosX - moveTo) * MOV_SPEED * deltaTime;
    if ((int64_t)round(l1PosX) == moveTo) l1PosX = moveTo;
}

void Shop::render(Window& window)
{
    BShop.render(window); // Shop button

    if (active)
    {
        Vect<int> textSizeInt = textSize.cast<int>();
        SDL_Rect dest = {50, 120, textSizeInt.x, textSizeInt.y};
        window.render(text, dest);
    }

    Vect<int> l1SizeInt = l1Size.cast<int>();
    SDL_Rect dest = {static_cast<int>(l1PosX), 0, l1SizeInt.x, l1SizeInt.y};
    window.drawRect(dest, {255, 255, 255});
}