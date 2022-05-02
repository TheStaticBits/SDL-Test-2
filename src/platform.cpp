#include "platform.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

Platform::Platform(const int tileWidth)
    : Interactable({TILE_SIZE * tileWidth, TILE_SIZE}, {tileWidth, 1}, {0, 255, 0})
{

}

Platform::~Platform()
{

}

bool Platform::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    return genCanPlace(pos, objects, size);
}

void Platform::update()
{

}

void Platform::render(Window& window, Vect<int> renderOffset)
{
    std::cout << "Child" << std::endl;
    // Add rendering for the supports of the platform
    Interactable::renderTiles(window, renderOffset);
}