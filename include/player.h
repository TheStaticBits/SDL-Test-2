#pragma once

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"
#include "interactable.h"

class Player
{
public:
    Player(Vect<float> pos);
    ~Player();

    void operator=(const Player&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool>& keys, 
                Base& base, 
                float deltaTime);
    void render(Window& window, Vect<int> renderOffset);
    
    inline Vect<int> getRenderOffset() const { return Vect<int>(static_cast<int>(renderOffset.x),
                                                                static_cast<int>(renderOffset.y)); }

private:
    static constexpr int SPEED = 80; // Pixels per second
    static constexpr float CAM_TIGHTNESS = 4.0f; // How tight the camera is on the player (higher is tighter)

    static constexpr int JUMP_SPEED = -400; 
    static constexpr int GRAVITY = -800; 

    Vect<float> pos;
    Vect<int> size;
    Vect<float> renderOffset;
    Vect<float> velocity;

    bool jump;

    Vect<float> getOffset();

    void collisions(Base& base);
};