#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"

namespace util
{
    inline Vect<int> getImgSize(SDL_Texture* texture)
    {
        Vect<int> result(0, 0);
        SDL_QueryTexture(texture, NULL, NULL, &result.x, &result.y);

        return result;
    }

    template <typename T>
    inline void lock(T& number, T max, T min)
    {
        if (number < min) number = min;
        else if (number > max) number = max;
    }

    inline bool collide(const SDL_Rect& rect1, const SDL_Rect& rect2)
    {
        return (rect1.x < rect2.x + rect2.w &&
                rect1.y < rect2.y + rect2.h &&
                rect2.x < rect1.x + rect1.w &&
                rect2.y < rect1.y + rect1.h);
    }
};