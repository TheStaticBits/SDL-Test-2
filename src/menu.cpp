#include "menu.h"

#include <iostream>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "button.h"
#include "utility.h"

Menu::Menu(const Vect<int64_t> menuPos, const Vect<uint32_t> menuSize)
    : bg(NULL), menuSize(menuSize), menuPos(menuPos)
{

}

Menu::Menu(const Vect<int64_t> menuPos, const nlohmann::json& menuData)
    : bg(NULL), menuSize(menuData["size"][0], menuData["size"][1]), menuPos(menuPos)
{
    
}

Menu::~Menu()
{
    if (bg != NULL) SDL_DestroyTexture(bg);
}

void Menu::addButton(Window& window, 
                     const std::string bName, std::string texType, 
                     Vect<int64_t> posOffset, std::string text,
                     const uint32_t fontSize, SDL_Color textColor,
                     Dir dirCentered)
{
    const Vect<int64_t> pos = getCenteredPos(Button::getButtonSize(texType), posOffset, dirCentered);
    buttons.emplace(bName, Button(window, texType, pos, text, fontSize, textColor));
}

void Menu::addText(Window& window, const uint32_t size, 
                   std::string text, SDL_Color color,
                   Vect<int64_t> posOffset, Dir dirCentered)
{
    SDL_Texture* texture = window.getTextImg(window.font(size), text, color);
    
    const Vect<uint32_t> textSize = util::getSize(texture);
    const Vect<int64_t> pos = getCenteredPos(textSize, posOffset, dirCentered);

    constText.emplace(text, std::make_pair(texture, util::getRect(pos, textSize)));
}

void Menu::update(Window& window)
{
    for (auto& button : buttons)
        button.second.update(window);
}

void Menu::render(Window& window)
{
    SDL_Rect bgRect = util::getRect(menuPos, menuSize);
    // if (bg != NULL) window.render(bg, bgRect);
    // else 
        window.drawRect(bgRect, { 255, 255, 255, 255 });

    for (auto& button : buttons)
        button.second.render(window);
    
    for (auto& text : constText)
        window.render(text.second.first, text.second.second);
}

void Menu::setX(const int64_t x)
{
    int64_t moved = x - menuPos.x;
    addX(moved);
}

void Menu::setY(const int64_t y)
{
    int64_t moved = y - menuPos.y;
    addY(moved);
}

void Menu::addX(const int64_t x)
{
    menuPos.x += x;
    for (auto& button : buttons) button.second.addX(x);
    for (auto& text : constText) text.second.second.x += x;
}

void Menu::addY(const int64_t y)
{
    menuPos.y += y;
    for (auto& button : buttons) button.second.addY(y);
    for (auto& text : constText) text.second.second.y += y;
}

const Vect<int64_t> Menu::getCenteredPos(const Vect<uint32_t> objSize, 
                                         const Vect<int64_t> posOffset, 
                                         const Dir dirCentered)
{
    switch (dirCentered)
    {
        case (None): return posOffset + menuPos;

        case (Top):
            return { menuPos.x + (menuSize.x - objSize.x) / 2 + posOffset.x, 
                     menuPos.y - posOffset.y };
        case (Bottom):
            return { menuPos.x + (menuSize.x - objSize.x) / 2 + posOffset.x, 
                     menuPos.y - posOffset.y };
        case(Left):
            return { menuPos.x + posOffset.x, 
                     menuPos.y + (menuSize.y - objSize.y) / 2 + posOffset.y };
        case(Right):
            return { menuPos.x - posOffset.x, 
                     menuPos.y + (menuSize.y - objSize.y) / 2 + posOffset.y };
        
        case (TopLeft): 
            return posOffset + menuPos;
        case (TopRight): 
            return { menuPos.x - objSize.x - posOffset.x, menuPos.y + posOffset.y };
        case (BottomLeft): 
            return { menuPos.x + posOffset.x, menuPos.y - objSize.y - posOffset.y };
        case (BottomRight):
            return { menuPos.x - objSize.x - posOffset.x, menuPos.y - objSize.y - posOffset.y };
        
        default:
            std::cout << "Direction not implemented: " << dirCentered << std::endl;
            return { 0, 0 };
    }
}