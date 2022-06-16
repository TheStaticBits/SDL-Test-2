#include "animation.h"

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "utility.h"
#include "vector.h"

Animation::Animation(SDL_Texture* tex, uint32_t frames, float delay)
    : texture(tex), delay(delay),
      totalFrames(frames), 
      size(util::getSize(texture) * SCALE),
      frameSize(size.x / totalFrames, size.y),
      delayCounter(0), frame(0), finished(false)
{
    
}

Animation::~Animation()
{

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
    SDL_Rect src = getSourceRect();
    SDL_Rect dst = getDestRect(pos);

    window.render(texture, src, dst);
}

void Animation::render(Window& window, SDL_Rect src, SDL_Rect dst)
{
    window.render(texture, src, dst);
}

void Animation::renderCenter(Window& window, const Vect<int64_t> center)
{
    Vect<int64_t> topLeft = center - (frameSize.cast<int64_t>() / 2);
    render(window, topLeft);
}

void Animation::modColor(Window& window, const std::vector<uint8_t> color)
{
    SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);
}

void Animation::modAlpha(Window& window, const uint8_t alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

void Animation::reset()
{
    delayCounter = 0;
    frame = 0;
    finished = false;
}

const SDL_Rect Animation::getSourceRect() const
{
    const Vect<int> frameSizeInt = frameSize.cast<int>();
    
    return { static_cast<int>(frame * frameSize.x), 0, 
             frameSizeInt.x, frameSizeInt.y };
}

const SDL_Rect Animation::getDestRect(const Vect<int64_t> pos) const
{
    const Vect<int> frameSizeInt = frameSize.cast<int>();
    const Vect<int> posInt = pos.cast<int>();

    return { posInt.x, posInt.y, frameSizeInt.x, frameSizeInt.y };
}