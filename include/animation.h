#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

class Animation
{
public:
    Animation(SDL_Texture* texture, uint32_t totalFrames, float delay);
    ~Animation();
    
    void operator=(const Animation&) = delete;

    void render(Window& window);

private:
    SDL_Texture* texture;
    
    const uint32_t totalFrames;
    const float delay; // Seconds

    uint32_t frame;
    float delayCounter;
};