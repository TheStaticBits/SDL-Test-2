#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "base.h"
#include "interactable.h"

class Platform : public Interactable
{
public:
    Platform(const int tileWidth);
    ~Platform();

    void operator=(const Platform&) = delete;

    bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& otherObjects);

    void update();

    void render(Window& window, Vect<int> renderOffset);

private:

};