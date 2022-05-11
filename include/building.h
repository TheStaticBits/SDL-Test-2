#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "base.h"
#include "interactable.h"

// Contains more specific code, for each building
// Buildings will inherit from this class
class Building : public Interactable
{
public:
    Building(const nlohmann::json& data, const Vect<int> tileSize, const std::vector<Uint8> color, const ObjType type);
    Building(const nlohmann::json& data, const std::vector<Uint8> color, const ObjType type); // Loading from save
    ~Building();

    void operator=(const Building&) = delete;

    bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size) override;

    virtual void frameUpdate(const std::time_t& seconds) = 0;
    void update(const std::time_t seconds) override; // Calls frameUpdate
    void render(Window& window, const Vect<int> renderOffset) override;

    std::string buildingGetSave();
    std::string buildingReadSave(const std::string& save);

    void placeDown() override;

protected:
    bool beingBuilt;
    bool updating; // Building is updating
    int buildingTimer;
    int level; 

    const nlohmann::json& data; // Data for the specific building
};