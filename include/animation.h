#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Window;
#include "vector.h"

class Animation
{
public:
    Animation(SDL_Texture* texture, uint32_t totalFrames, float delay);
    ~Animation();
    
    void operator=(const Animation&) = delete;

    void update(Window& window);
    void render(Window& window, const Vect<int64_t> pos);
    void renderCenter(Window& window, const Vect<int64_t> center);

    const bool isFinished() const { return finished; }

private:
    SDL_Texture* texture;
    
    const float delay; // Seconds
    const uint32_t totalFrames;
    const Vect<uint32_t> size;
    const Vect<uint32_t> frameSize;

    float delayCounter;
    uint32_t frame;

    bool finished; 
};