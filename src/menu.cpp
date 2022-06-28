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

Menu::Menu(Window& window, const Vect<int64_t> menuPos, const nlohmann::json& menuData)
    : bg(window.loadTexture(menuData["bgPath"].get<std::string>().c_str())), 
      menuSize(util::getSize(bg)), menuPos(menuPos)
{
    // Loading menu from JSON data given

    for (const auto& button : menuData["buttons"].items())
    {
        // Parse json for button data
        addButton(window, button.key(), button.value()["texture"].get<std::string>(),
                                        Vect<int64_t>{ button.value()["offset"][0].get<int64_t>(),
                                                       button.value()["offset"][1].get<int64_t>() },
                                        button.value()["text"]["text"].get<std::string>(),
                                        button.value()["text"]["fontSize"].get<uint32_t>(),
                                        SDL_Color( button.value()["text"]["color"][0].get<uint8_t>(),
                                                   button.value()["text"]["color"][1].get<uint8_t>(),
                                                   button.value()["text"]["color"][2].get<uint8_t>(),
                                                   button.value()["text"]["color"][3].get<uint8_t>() ),
                                        strToDir.at(button.value()["lockDir"].get<std::string>()));
    }

    for (const auto& text : menuData["staticText"].items())
    {
        addText(window, text.key(), text.value()["fontSize"].get<uint32_t>(),
                                    text.value()["text"].get<std::string>(),
                                    SDL_Color( text.value()["color"][0].get<uint8_t>(),
                                               text.value()["color"][1].get<uint8_t>(),
                                               text.value()["color"][2].get<uint8_t>(), 
                                               text.value()["color"][3].get<uint8_t>() ),
                                    Vect<int64_t>{ text.value()["offset"][0].get<int64_t>(),
                                                   text.value()["offset"][1].get<int64_t>() },
                                    strToDir.at(text.value()["lockDir"].get<std::string>()));
    }
}

Menu::~Menu()
{
    //if (bg != NULL) SDL_DestroyTexture(bg);
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

void Menu::addText(Window& window, 
                   const std::string tName, const uint32_t size, 
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
    SDL_Rect bgRect = getRect();
    if (bg != NULL) window.render(bg, bgRect);
    else window.drawRect(bgRect, { 255, 255, 255, 255 });

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
    for (auto& text : constText) text.second.second.x += static_cast<int>(x);
}

void Menu::addY(const int64_t y)
{
    menuPos.y += y;
    for (auto& button : buttons) button.second.addY(y);
    for (auto& text : constText) text.second.second.y += static_cast<int>(y);
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
                     menuPos.y + posOffset.y };
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