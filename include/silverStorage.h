#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "building.h"
#include "interactable.h"
#include "vector.h"

class SilverStorage : public Building
{
public:
    SilverStorage(const nlohmann::json& data); // Creation function
    SilverStorage(const nlohmann::json& data, std::string save); // Loading from save
    ~SilverStorage();

    void operator=(const SilverStorage&) = delete;
    
    void update(const uint64_t& time) override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;

private:

};