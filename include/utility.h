#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"

namespace util
{
    inline Vect<uint32_t> getSize(SDL_Texture* texture)
    {
        Vect<int> result(0, 0);
        SDL_QueryTexture(texture, NULL, NULL, &result.x, &result.y);
        return result.cast<uint32_t>();
    }

    inline Vect<uint32_t> getSize(TTF_Font*& font, std::string text)
    {
        Vect<int> result(0, 0);
        TTF_SizeText(font, text.c_str(), &result.x, &result.y);
        return result.cast<uint32_t>();
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

    inline bool collide(const SDL_Rect& rect, const Vect<int64_t> point)
    {
        return (point.x >= rect.x          &&
                point.x <= rect.x + rect.w &&
                point.y >= rect.y          &&
                point.y <= rect.y + rect.h);
    }

    inline std::vector<std::string> split(std::string str, const std::string& delim)
    {
        std::vector<std::string> result;

        size_t pos = 0;
        while ((pos = str.find(delim)) != std::string::npos)
        {
            result.push_back(str.substr(0, pos));
            str.erase(0, pos + delim.length());
        }

        result.push_back(str);

        return result;
    }

    // template <typename T, typename O>
    inline SDL_Rect getRect(const Vect<int64_t> pos, const Vect<uint32_t> size)
    {
        SDL_Rect rect = { pos.xCast<int>(), pos.yCast<int>(), 
                          size.xCast<int>(), size.yCast<int>() };
        return rect;
    }
};