#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

class Window;
#include "building.h"

class SilverStorage : public Building
{
public:
    SilverStorage(Window& window, const nlohmann::json& data); // Creation function
    SilverStorage(Window& window, const nlohmann::json& data, std::string save); // Loading from save
    ~SilverStorage();

    void operator=(const SilverStorage&) = delete;
    
    void update(Window& window, const uint64_t& time) override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;

    inline const uint64_t getSilver() { return capacity; }
    
    const uint64_t addSilver(uint64_t add);

private:
    uint64_t capacity;
};