#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"

class Base
{
public:
    Base();
    ~Base();

    void operator=(const Base&) = delete;

    Vect<int> getSize() const { return size; }

private:
    Vect<int> size;
};