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
      l1Pos(WIN_WIDTH, WIN_HEIGHT / 2 - l1Size.y / 2),
      l1OutX(static_cast<int64_t>(WIN_WIDTH - l1Size.x)),

      l2Bg(window.loadTexture("res/shop/l2Bg.png")),
      l2Size(util::getSize(l2Bg)),
      l2Pos(WIN_WIDTH, WIN_HEIGHT / 2 - l2Size.y / 2),
      locked(true),

      text(window.getTextImg(window.font(10), "Shop", {0, 0, 0, 255})),
      textSize(util::getSize(text)), 
      
      shopButton(window, ShopB),
      buildingsButton(window, BuildingsB),
      platformsButton(window, BuildingsB), // Temporary
      
      active(false),
      openCategory(NoneOpen),
      switchToCategory(NoneOpen)
{
    shopButton.setPos(Vect<int64_t>(WIN_WIDTH - shopButton.getSize().x - 1, 1));

    buildingsButton.setY(l1Pos.y + shopButton.getPos().y + 20);
    platformsButton.setY(l1Pos.y + shopButton.getPos().y + buildingsButton.getPos().y + 40);
}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(l1Bg);
    SDL_DestroyTexture(l2Bg);
}

void Shop::update(const Vect<int64_t>& mousePos,    
                  std::unordered_map<uint8_t, bool>& mouseButtons,
                  std::unordered_map<uint8_t, bool>& mouseHeldButtons,
                  const float deltaTime)
{
    // Shop sliding graphics
    moveL1(deltaTime);
    moveL2(deltaTime);
    
    // Updating buttons
    // Open/close menu
    shopButton.update(mousePos, mouseHeldButtons);
    if (shopButton.isActivated()) toggleShop();

    buildingsButton.setX(l1Pos.x + (l1Size.x / 2)
                         - (buildingsButton.getSize().x / 2));
    buildingsButton.update(mousePos, mouseHeldButtons);
    if (buildingsButton.isActivated()) switchCategory(Buildings);

    platformsButton.setX(l1Pos.x + (l1Size.x / 2)
                         - (platformsButton.getSize().x / 2));
    platformsButton.update(mousePos, mouseHeldButtons);
    if (platformsButton.isActivated()) switchCategory(Platforms);
}

void Shop::render(Window& window)
{
    shopButton.render(window); // Shop button

    Vect<int> l1SizeInt =   l1Size.cast<int>();
    Vect<int> l1PosInt =    l1Pos.cast<int>();
    Vect<int> l2PosInt =    l2Pos.cast<int>();
    Vect<int> l2SizeInt =   l2Size.cast<int>();
    Vect<int> textSizeInt = textSize.cast<int>();

    SDL_Rect dest;

    // Rendering shop background
    dest = {l2PosInt.x, l2PosInt.y, l2SizeInt.x, l2SizeInt.y};
    window.render(l2Bg, dest);

    dest = {l1PosInt.x, l1PosInt.y, l1SizeInt.x, l1SizeInt.y};
    window.render(l1Bg, dest);

    // Rendering text
    dest = {l1PosInt.x + (l1SizeInt.x / 2) - (textSizeInt.x / 2), 
            l1PosInt.y + 5, textSizeInt.x, textSizeInt.y};
    window.render(text, dest);
    
    // Layer 1 Buttons
    buildingsButton.render(window);
    platformsButton.render(window);
}

void Shop::toggleShop()
{
    if (active) 
    {
        active = false;
        if (openCategory != NoneOpen)
        {
            openCategory = NoneOpen;
            locked = false;
        }
    }
    else active = true;
}

void Shop::switchCategory(Category category)
{
    locked = false;
    if (openCategory != category) 
    {
        if (openCategory != NoneOpen)
        {
            openCategory = NoneOpen;
            switchToCategory = category;
            locked = false;
        }
        else openCategory = category;
    }
    else 
        openCategory = NoneOpen;
}

void Shop::moveL1(const float& deltaTime)
{
    int64_t moveTo = active ? l1OutX : WIN_WIDTH;
    if ((int64_t)round(l1Pos.x) != moveTo)
        l1Pos.x -= (l1Pos.x - moveTo) * MOV_SPEED * deltaTime;
    else
        l1Pos.x = moveTo;
}

void Shop::moveL2(const float& deltaTime)
{
    int64_t moveTo;
    if (openCategory == NoneOpen)
    {
        if (switchToCategory != NoneOpen) moveTo = WIN_WIDTH;
        else moveTo = l1Pos.x;
    }
    else moveTo = WIN_WIDTH - l2Size.x;

    if (!locked && ((int64_t)round(l2Pos.x) != moveTo))
        l2Pos.x -= (l2Pos.x - moveTo) * MOV_SPEED * deltaTime;
    else 
    {
        l2Pos.x = moveTo;
        locked = true;
    }

    if ((switchToCategory != NoneOpen) && (l2Pos.x >= l1Pos.x))
    {
        l2Pos.x = l1Pos.x;
        openCategory = switchToCategory;
        switchToCategory = NoneOpen;
        locked = false;
    }
}