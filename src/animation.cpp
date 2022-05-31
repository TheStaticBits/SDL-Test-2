#include "animation.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "utility.h"

Animation::Animation(SDL_Texture* texture, uint32_t totalFrames, float delay)
    : texture(texture), delay(delay),
      totalFrames(totalFrames), 
      size(util::getSize(texture)),
      frameSize(size.x / totalFrames, size.y),
      delayCounter(0), frame(0), finished(false)
{

}

Animation::~Animation()
{
    SDL_DestroyTexture(texture);
}

void Animation::update(Window& window)
{
    finished = false;

    delayCounter += window.getDeltaTime();
    if (delayCounter >= delay)
    {
        delayCounter = 0.0f;

        if (++frame >= totalFrames)
        {
            finished = true;
            frame = 0;
        }
    }
}

void Animation::render(Window& window, const Vect<int64_t> pos)
{
    Vect<int> frameSizeInt = frameSize.cast<int>();
    Vect<int> posInt = pos.cast<int>();

    SDL_Rect src = { static_cast<int>(frame * frameSize.x), 0, 
                     frameSizeInt.x, frameSizeInt.y };
    SDL_Rect dst = { posInt.x, posInt.y, 
                     frameSizeInt.x, frameSizeInt.y };
    
    window.render(texture, src, dst);
}