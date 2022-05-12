#pragma once

#include <iostream>
#include <string>
#include <ctime>

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

    std::string getSave() override;
    void readSave(const std::string& save) override;

    void updateFrame(const std::time_t& seconds) override;

private:

};