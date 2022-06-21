#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Window;
#include "vector.h"

class Animation
{
public:
    Animation(SDL_Texture* tex, uint32_t frames, float delay);
    ~Animation();
    
    void operator=(const Animation&) = delete;

    void update(Window& window);
    void render(Window& window, const Vect<int64_t> pos);
    void render(Window& window, SDL_Rect src, SDL_Rect dst);
    void renderCenter(Window& window, const Vect<int64_t> center);

    void reset();

    void modColor(Window& window, const std::vector<uint8_t> color);
    void modAlpha(Window& window, const uint8_t alpha);

    inline const bool isFinished() const         { return finished;  }
    inline const Vect<uint32_t> getSize() const  { return size;      }
    inline const Vect<uint32_t> getFrame() const { return frameSize; }
    inline SDL_Texture* getTexture() const       { return texture;   }
    inline const uint32_t getFrameNum() const    { return frame;     }

    const SDL_Rect getSourceRect() const;
    const SDL_Rect getDestRect(const Vect<int64_t> pos) const;

    inline void setTex(SDL_Texture* newTex) { texture = newTex; }

    inline void flip() { flipped = !flipped; }
    
    inline void lock()   { locked = true;  }
    inline void unlock() { locked = false; }
    
    inline void sendToOpposite() { frame = ((frame == 0) ? (totalFrames - 1) : 0); }

private:
    SDL_Texture* texture;
    
    const float delay; // Seconds
    const uint32_t totalFrames;
    const Vect<uint32_t> size;
    const Vect<uint32_t> frameSize;

    float delayCounter;
    uint32_t frame;

    bool flipped;
    bool locked;
    bool finished; 
};