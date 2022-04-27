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
};