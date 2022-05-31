#include "animation.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Animation::Animation(SDL_Texture* texture, uint32_t totalFrames, float delay)
    : texture(texture), totalFrames(totalFrames), delay(delay),
      frame(0), delayCounter(0)
{

}

Animation::~Animation()
{

}

void Animation::render(Window& window)
{

}