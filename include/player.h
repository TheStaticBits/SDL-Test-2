#pragma once

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"

class Player
{
public:
    Player(Vect<float> pos);
    ~Player();

    void operator=(const Player&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool> keys);
    void render(Window& window, Vect<int> renderOffset);
    
    inline Vect<int> getRenderOffset() const { return Vect<int>(static_cast<int>(renderOffset.x),
                                                                static_cast<int>(renderOffset.y)); }

private:
    static const int speed = 2;

    Vect<float> pos;
    Vect<int> size;
    Vect<float> renderOffset;

    Vect<float> getOffset();
};