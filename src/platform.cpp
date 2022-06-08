#include "platform.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

Platform::Platform(Window& window, const nlohmann::json& data, const uint32_t tileWidth)
    : Interactable(window, data, {tileWidth, 1}, Platform_T)
{

}

Platform::Platform(Window& window, const nlohmann::json& data, std::string save)
    : Interactable(window, data, Platform_T)
{
    modColor = {0, 255, 0};

    save = Interactable::readSave(save);
    readSave(save);
}

Platform::~Platform()
{

}

bool Platform::canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size)
{
    return Interactable::canPlace(pos, objects, size);
}

// void Platform::update(const uint64_t& seconds)
// {

// }

void Platform::render(Window& window, const Vect<int64_t>& renderOffset)
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