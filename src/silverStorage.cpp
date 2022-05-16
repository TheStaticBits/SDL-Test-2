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
    
    save = Interactable::readSave(save);
    save = Building::readSave(save);
    readSave(save);
}

SilverStorage::~SilverStorage()
{

}

void SilverStorage::update(const uint64_t& time)
{
    Building::update(time);

    if (!beingBuilt)
    {
        // Functionality
    }
}

std::string SilverStorage::getSave()
{
    std::string save = Interactable::getSave();
    save += Building::getSave();
    
    // Add anything nessecary

    return save;
}

std::string SilverStorage::readSave(std::string& save)
{
    return ""; // No more data to parse
}