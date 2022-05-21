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
    : l1Bg(window.loadTexture("res/shop/l1Bg.png")),
      l1Size(util::getSize(l1Bg)),
      l1OutX(static_cast<int64_t>(WIN_WIDTH - l1Size.x)),
      text(window.getTextImg(window.font(10), "Shop", {0, 0, 0, 255})),
      textSize(util::getSize(text)), 
      shopButton(window, ShopB, Vect<int64_t>()),
      active(false), 
      l1Pos(WIN_WIDTH, WIN_HEIGHT / 2 - l1Size.y / 2)
      // l2Pos(WIN_WIDTH, WIN_HEIGHT / 2 - l1Size.y / 2)
{
    const Vect<uint32_t> shopSize = shopButton.getSize();
    shopButton.setPos(Vect<int64_t>(WIN_WIDTH - shopSize.x - 1, 1));
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
    shopButton.update(mousePos, mouseHeldButtons);
    if (shopButton.isActivated()) active = !active;

    // Shop sliding graphics
    int64_t moveTo = active ? l1OutX : WIN_WIDTH;
    l1Pos.x -= (l1Pos.x - moveTo) * MOV_SPEED * deltaTime;
    if ((int64_t)round(l1Pos.x) == moveTo) l1Pos.x = moveTo;
}

void Shop::render(Window& window)
{
    shopButton.render(window); // Shop button

    Vect<int> l1SizeInt = l1Size.cast<int>();
    Vect<int> l1PosInt = l1Pos.cast<int>();
    Vect<int> textSizeInt = textSize.cast<int>();

    SDL_Rect dest;

    // Rendering shop background
    dest = {l1PosInt.x, l1PosInt.y, l1SizeInt.x, l1SizeInt.y};
    window.render(l1Bg, dest);

    // Rendering text
    dest = {l1PosInt.x + (l1SizeInt.x / 2) - (textSizeInt.x / 2), 
            l1PosInt.y + 5, textSizeInt.x, textSizeInt.y};
    window.render(text, dest);
}