#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

enum bTextures { BlueB, ShopB, BuildingsB};
inline std::unordered_map<bTextures, std::string> bFolderNames = {
    { BlueB,      "blue"      },
    { ShopB,      "shop"      },
    { BuildingsB, "buildingB" }
};
inline std::vector<std::string> bImgStates = { "idle", "hovering", "pressed" };

class Button
{
public:
    Button(Window& window, bTextures texType, 
           Vect<int64_t> pos = {0, 0}, std::string text = "",
           const uint32_t fontSize = 0,
           SDL_Color textColor = {255, 255, 255, 255});
    ~Button();

    void operator=(const Button&) = delete;

    void setupText(std::string text, const uint32_t fontSize, SDL_Color color);

    void update(Window& window);
    void render(Window& window, Vect<int64_t> textOffset = {0, 0});
    
    inline void setPos(Vect<int64_t> newPos) { pos = newPos; }
    inline void setX(int64_t x)              { pos.x = x;    }
    inline void setY(int64_t y)              { pos.y = y;    }

    inline const bool isActivated()       const { return activated; }
    inline const Vect<uint32_t> getSize() const { return size;      }
    inline const Vect<int64_t> getPos()   const { return pos;       }
    inline const SDL_Rect getRect()       const { return rect;      }

private:
    static std::unordered_map<bTextures, std::unordered_map<std::string, SDL_Texture*>> textures;
    
    bTextures texType;
    Vect<uint32_t> size;

    SDL_Texture* textImg;
    Vect<uint32_t> textSize;

    Vect<int64_t> pos;

    SDL_Rect rect;
    void updateRect();

    bool hovering;
    bool pressed;
    bool activated;
};