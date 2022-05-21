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

enum Category { NoneOpen, Buildings, Platforms, Robots };

class Shop
{
public:
    Shop(Window& window);
    ~Shop();

    void operator=(const Shop&) = delete;

    void update(const Vect<int64_t>& mousePos,    
                std::unordered_map<uint8_t, bool>& mouseButtons,
                std::unordered_map<uint8_t, bool>& mouseHeldButtons,
                const float deltaTime);
    void render(Window& window);

    inline const bool isActive() const { return active; }

private:
    static constexpr float MOV_SPEED = 9.0f;

    SDL_Texture* l1Bg;
    Vect<uint32_t> l1Size;
    Vect<float> l1Pos;
    const int64_t l1OutX;

    SDL_Texture* l2Bg;
    Vect<uint32_t> l2Size;
    Vect<float> l2Pos;
    bool locked;

    SDL_Texture* text;
    Vect<uint32_t> textSize;

    Button shopButton;
    Button buildingsButton;
    Button platformsButton;
    
    bool active;
    Category openCategory;
    Category switchToCategory;

    void toggleShop();
    void switchCategory(Category category);

    void moveL1(const float& deltaTime);
    void moveL2(const float& deltaTime);
};