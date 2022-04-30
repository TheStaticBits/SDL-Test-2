#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "interactable.h"

class Base
{
public:
    Base();
    ~Base();

    void operator=(const Base&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool>& keys, 
                std::unordered_map<SDL_Keycode, bool>& oneTimeKeys,
                std::unordered_map<Uint8, bool>& mouseButtons, 
                Vect<int>& mousePos,
                Vect<int>& renderOffset);

    void render(Window& window, Vect<int> renderOffset);

    inline Vect<int> getSize() const { return size; }

private:
    static constexpr int TILE_SIZE = 20;

    Vect<int> size;

    std::vector<Interactable> objects;

    bool placing;
};