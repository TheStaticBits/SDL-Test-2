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
      l1Pos(window.getSize().x, window.getSize().y / 2 - l1Size.y / 2),
      l1OutX(static_cast<int64_t>(window.getSize().x - l1Size.x)),

      l2Bg(window.loadTexture("res/shop/l2Bg.png")),
      l2Size(util::getSize(l2Bg)),
      l2Pos(window.getSize().x, window.getSize().y / 2 - l2Size.y / 2),
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
    updateUIPositions(window);
}

Shop::~Shop()
{
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(l1Bg);
    SDL_DestroyTexture(l2Bg);
}

void Shop::update(Window& window, const float deltaTime)
{
    // Shop sliding graphics
    moveL1(window, deltaTime);
    moveL2(window, deltaTime);
    
    // Updating buttons
    // Open/close menu
    shopButton.update(window);
    if (shopButton.isActivated()) toggleShop();

    buildingsButton.setX(l1Pos.x + (l1Size.x / 2)
                         - (buildingsButton.getSize().x / 2));
    buildingsButton.update(window);
    if (buildingsButton.isActivated()) switchCategory(Buildings);

    platformsButton.setX(l1Pos.x + (l1Size.x / 2)
                         - (platformsButton.getSize().x / 2));
    platformsButton.update(window);
    if (platformsButton.isActivated()) switchCategory(Platforms);

    updateRects();

    const Vect<int64_t> mousePos = window.getMousePos();
    
    if (active && window.buttonHeld(SDL_BUTTON_LEFT))
    {
        if (!util::collide(l1Rect, mousePos) && 
            !util::collide(l2Rect, mousePos) && 
            !util::collide(shopButton.getRect(), mousePos))
            toggleShop();
        else
            window.setButton(SDL_BUTTON_LEFT, false);
    }
}

void Shop::render(Window& window)
{
    shopButton.render(window); // Shop button

    // Rendering shop background
    window.render(l2Bg, l2Rect);
    window.render(l1Bg, l1Rect);

    // Rendering text
    window.render(text, textRect);
    
    // Layer 1 Buttons
    buildingsButton.render(window);
    platformsButton.render(window);
}

void Shop::resize(const Window& window)
{
    Vect<uint32_t> winSize = window.getSize();

    float xChange = winSize.x - l1OutX - l1Size.x;
    l1OutX = winSize.x - l1Size.x;

    l1Pos.y = winSize.y / 2 - l1Size.y / 2; l1Pos.x += xChange;
    l2Pos.y = winSize.y / 2 - l2Size.y / 2; l2Pos.x += xChange;

    updateUIPositions(window);
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

    if (openCategory == category) 
        openCategory = NoneOpen;
    else if (openCategory == NoneOpen) 
        openCategory = category;
    else
    {
        openCategory = NoneOpen;
        switchToCategory = category;
    } 
}

void Shop::updateRects()
{
    Vect<int> l1SizeInt =   l1Size.cast<int>();
    Vect<int> l1PosInt =    l1Pos.cast<int>();
    Vect<int> l2PosInt =    l2Pos.cast<int>();
    Vect<int> l2SizeInt =   l2Size.cast<int>();
    Vect<int> textSizeInt = textSize.cast<int>();

    l1Rect = {l1PosInt.x, l1PosInt.y, l1SizeInt.x, l1SizeInt.y};
    l2Rect = {l2PosInt.x, l2PosInt.y, l2SizeInt.x, l2SizeInt.y};

    textRect = {l1PosInt.x + (l1SizeInt.x / 2) - (textSizeInt.x / 2), 
                l1PosInt.y + 5, textSizeInt.x, textSizeInt.y};
}

void Shop::updateUIPositions(const Window& window)
{
    shopButton.setPos(Vect<int64_t>(window.getSize().x - shopButton.getSize().x - 1, 1));

    buildingsButton.setY(l1Pos.y + 20);
    platformsButton.setY(buildingsButton.getPos().y + buildingsButton.getSize().y + 10);
}

void Shop::moveL1(const Window& window, const float& deltaTime)
{
    int64_t moveTo = active ? l1OutX : window.getSize().x;
    if ((int64_t)round(l1Pos.x) != moveTo)
        l1Pos.x -= (l1Pos.x - moveTo) * MOV_SPEED * deltaTime;
    else
        l1Pos.x = moveTo;
}

void Shop::moveL2(const Window& window, const float& deltaTime)
{
    int64_t moveTo;
    if (openCategory == NoneOpen)
    {
        if (switchToCategory != NoneOpen) moveTo = window.getSize().x;
        else moveTo = l1Pos.x;
    }
    else moveTo = window.getSize().x - l2Size.x;

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