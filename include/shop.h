#pragma once

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"
#include "button.h"

class Shop
{
public:
    Shop(Window& window);
    ~Shop();

    void operator=(const Shop&) = delete;

    void update(const Vect<int64_t>& mousePos,    
                const std::unordered_map<uint8_t, bool>& mouseButtons);
    void render(Window& window);

    inline const bool isActive() const { return active; }

private:
    SDL_Texture* text;
    Vect<uint32_t> textSize;
    
    bool active;

    int64_t l1PosX;
    int64_t l2PosX;
};