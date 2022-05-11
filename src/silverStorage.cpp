#include "silverStorage.h"

#include <iostream>
#include <string>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "building.h"
#include "interactable.h"

SilverStorage::SilverStorage(const nlohmann::json& data)
    : Building(data, {data["size"][0], data["size"][1]}, {148, 148, 148}, SilverStorage_T)
{

}

// Loads from save file
SilverStorage::SilverStorage(const nlohmann::json& data, const std::string save)
    : Building(data, {148, 148, 148}, SilverStorage_T)
{
    save = Interactable::genReadSave(save);
    save = Building::buildingReadSave(save);
    readSave(save);

    // CHANGE SOON, so it loads the tile size from the JSON first or something
    renderPos.w = tileSize.x * TILE_SIZE;
    renderPos.h = tileSize.y * TILE_SIZE;
}

SilverStorage::~SilverStorage()
{

}

std::string SilverStorage::getSave()
{

}

void SilverStorage::readSave(std::string save)
{

}

void frameUpdate(const std::time_t& seconds)
{

}