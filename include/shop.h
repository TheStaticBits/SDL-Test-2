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
                std::unordered_map<uint8_t, bool>& mouseButtons,
                std::unordered_map<uint8_t, bool>& mouseHeldButtons,
                float deltaTime);
    void render(Window& window);

    inline const bool isActive() const { return active; }

private:
    static constexpr float MIN_SPEED = 50.0f; // tweak these
    static constexpr float MOV_SPEED = 2.0f;

    SDL_Texture* text;
    Vect<uint32_t> textSize;

    Vect<uint32_t> l1Size;

    Button BShop;
    
    bool active;

    float l1PosX;
    float l2PosX;
};