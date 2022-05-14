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
    : Interactable({tileWidth, 1}, {0, 255, 0}, Platform_T)
{

}

Platform::Platform(std::string save)
    : Interactable(Platform_T)
{
    renderColor = {0, 255, 0};

    save = Interactable::readSave(save);
    readSave(save);
}

Platform::~Platform()
{

}

bool Platform::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    return Interactable::canPlace(pos, objects, size);
}

void Platform::update(const uint64_t& seconds)
{

}

void Platform::render(Window& window, const Vect<int>& renderOffset)
{
    // Add rendering for the supports of the platform
    Interactable::render(window, renderOffset);
}

std::string Platform::getSave()
{
    std::string save = Interactable::getSave();

    save += std::to_string(tileSize.x);

    return save;
}

std::string Platform::readSave(std::string& save)
{
    tileSize.y = 1; // Height of platform is always 1
    tileSize.x = std::stoi(save);

    renderPos.w = tileSize.x * TILE_SIZE;
    renderPos.h = tileSize.y * TILE_SIZE;

    return ""; // No more save to read
}