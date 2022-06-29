#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <nlohmann/json.hpp>

class Window;
#include "vector.h"
#include "utility.h"
#include "animation.h"

class Button
{
public:
    Button(Window& window, std::string texType,
           Vect<int64_t> pos = {0, 0}, std::string text = "",
           const uint32_t fontSize = 0, SDL_Color textColor = {0, 0, 0});
    ~Button();

    static void loadButtonData(Window& window);
    static void resetTextures(Window& window);

    void setupText(std::string text, const uint32_t fontSize, SDL_Color color);

    void update(Window& window);
    void render(Window& window, Vect<int64_t> textOffset = {0, 0});
    
    inline void setPos(Vect<int64_t> newPos) { pos = newPos; }
    inline void setX(int64_t x)              { pos.x = x;    }
    inline void setY(int64_t y)              { pos.y = y;    }
    inline void addX(int64_t x)              { pos.x += x;   }
    inline void addY(int64_t y)              { pos.y += y;   }

    inline const bool isActivated()       const { return activated; }
    inline const Vect<uint32_t> getSize() const { return size;      }
    inline const Vect<int64_t> getPos()   const { return pos;       }
    inline const SDL_Rect getRect()       const { return rect;      }

    inline static const Vect<uint32_t> getButtonSize(std::string texType)
    {
        Vect<uint32_t> size = util::getSize(textures[texType]);
        size.x /= buttonData[texType]["anim"]["frames"].get<uint32_t>();
        return size;
    }

private:
    inline static const std::string DATA_PATH = "res/data/buttons.json";

    static nlohmann::json buttonData;
    static std::unordered_map<std::string, SDL_Texture*> textures;
    
    std::string texType;
    Animation animation;
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