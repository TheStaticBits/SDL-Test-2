#pragma once

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Window;
#include "vector.h"
#include "button.h"

enum Category { NoneOpen, Buildings, Platforms, Robots };

class Shop
{
public:
    Shop(Window& window);
    ~Shop();

    void operator=(const Shop&) = delete;

    void update(Window& window);
    void render(Window& window);

    void resize(const Window& window);

    inline const bool isActive() const { return active; }

private:
    static constexpr float MOV_SPEED = 9.0f;

    SDL_Texture* l1Bg;
    Vect<uint32_t> l1Size;
    Vect<float> l1Pos;
    SDL_Rect l1Rect;
    int64_t l1OutX;

    SDL_Texture* l2Bg;
    Vect<uint32_t> l2Size;
    Vect<float> l2Pos;
    SDL_Rect l2Rect;
    bool locked;

    SDL_Texture* text;
    Vect<uint32_t> textSize;
    SDL_Rect textRect;

    Button shopButton;
    Button buildingsButton;
    Button platformsButton;
    
    bool active;
    Category openCategory;
    Category switchToCategory;

    void toggleShop();
    void switchCategory(Category category);

    void updateRects();
    void updateUIPositions(const Window& window);

    void moveL1(const Window& window, const float& deltaTime);
    void moveL2(const Window& window, const float& deltaTime);
};