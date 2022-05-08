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
    : Interactable({TILE_SIZE * tileWidth, TILE_SIZE}, {tileWidth, 1}, {0, 255, 0}, PlatformType)
{

}

Platform::Platform(std::string save)
    : Interactable(PlatformType)
{
    save = genReadSave(save);
    readSave(save);
}

Platform::~Platform()
{

}

bool Platform::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData)
{
    return Interactable::genCanPlace(pos, objects, size);
}

void Platform::update(std::time_t seconds)
{

}

void Platform::render(Window& window, Vect<int> renderOffset)
{
    // Add rendering for the supports of the platform
    Interactable::genRender(window, renderOffset);
}

std::string Platform::getSave()
{
    return Interactable::genSaveData();
}

void Platform::readSave(std::string save)
{

}