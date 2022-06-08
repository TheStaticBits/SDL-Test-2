#include "silverStorage.h"

#include <iostream>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "window.h"
#include "building.h"
#include "interactable.h"
#include "vector.h"

// Create a new building
SilverStorage::SilverStorage(Window& window, const nlohmann::json& data)
    : Building(window, data, SilverStorage_T), capacity(0)
{
    
}

// Loads from save file
SilverStorage::SilverStorage(Window& window, const nlohmann::json& data, std::string save)
    : Building(window, data, SilverStorage_T)
{
    placing = false;
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
    
    save += std::to_string(capacity);

    return save;
}

std::string SilverStorage::readSave(std::string& save)
{
    save = Interactable::readSave(save);
    save = Building::readSave(save);

    capacity = std::stoull(save);

    return ""; // Nothing else to parse
}

const uint64_t SilverStorage::addSilver(uint64_t add)
{
    capacity += add;

    uint64_t amountOver = 0;
    uint64_t max = getLvlData()["storage"];
    
    if (capacity > max)
    {
        amountOver = capacity - max;
        capacity = max;
    }

    return amountOver;
}