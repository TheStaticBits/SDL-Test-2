#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Window;
#include "vector.h"
#include "button.h"
#include "utility.h"

enum Dir { None, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight };
inline const std::unordered_map<std::string, Dir> strToDir = {
    { "None", None },
    { "Top", Top },
    { "Bottom", Bottom },
    { "Left", Left },
    { "Right", Right },
    { "TopLeft", TopLeft },
    { "TopRight", TopRight },
    { "BottomLeft", BottomLeft },
    { "BottomRight", BottomRight }
};

class Menu
{
public:
    Menu(Window& window, const Vect<int64_t> menuPos, const nlohmann::json& menuData);
    ~Menu();

    // Add elements to the menu
    void addBg(SDL_Texture* tex) { bg = tex; }

    void addButton(Window& window, 
                   const std::string bName, std::string texType, 
                   Vect<int64_t> posOffset, std::string text = "",
                   const uint32_t fontSize = 0, SDL_Color textColor = { 255, 255, 255, 255 },
                   Dir dirCentered = None);

    void addText(Window& window, 
                 const std::string tName, const uint32_t size, 
                 std::string text, SDL_Color color = { 255, 255, 255, 255 },
                 Vect<int64_t> posOffset = { 0, 0 }, Dir dirCentered = None);

    // Update & render
    void update(Window& window);
    void render(Window& window);
    
    // Setters
    void setX(const int64_t x);
    void setY(const int64_t y);
    void addX(const int64_t x);
    void addY(const int64_t y);

    // Getters
    const Vect<int64_t> getCenteredPos(const Vect<uint32_t> objSize, 
                                       const Vect<int64_t> posOffset, 
                                       const Dir dirCentered = None);

    inline Button& getButton(const std::string button) { return buttons.at(button);               }
    const bool buttonPressed(const std::string button) { return buttons.at(button).isActivated(); }

    inline const Vect<int64_t> getPos() { return menuPos; }
    inline const SDL_Rect getRect() { return util::getRect(menuPos, menuSize); }

    inline const Vect<uint32_t> getSize() const { return menuSize; }
    inline const Vect<int64_t> getPos() const   { return menuPos;  }

private:
    SDL_Texture* bg;
    Vect<uint32_t> menuSize;
    Vect<int64_t> menuPos;

    std::unordered_map<std::string, Button> buttons;
    std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> constText;
};