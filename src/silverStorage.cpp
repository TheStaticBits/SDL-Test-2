#include "silverStorage.h"

#include <iostream>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "building.h"
#include "interactable.h"
#include "vector.h"

// Create a new building
SilverStorage::SilverStorage(const nlohmann::json& data)
    : Building(data, {148, 148, 148}, SilverStorage_T)
{

}

// Loads from save file
SilverStorage::SilverStorage(const nlohmann::json& data, std::string save)
    : Building(data, {148, 148, 148}, SilverStorage_T)
{
    placing = false;
    
    save = Interactable::genReadSave(save);
    save = Building::buildingReadSave(save);
    readSave(save);
}

SilverStorage::~SilverStorage()
{

}

std::string SilverStorage::getSave()
{
    std::string save = Interactable::genGetSave();
    save += Building::buildingGetSave();
    
    // Add anything nessecary

    return save;
}

void SilverStorage::readSave(const std::string& save)
{

}

void SilverStorage::updateFrame(const uint64_t& seconds)
{
    
}