#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

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
    Building(const nlohmann::json& data, const std::vector<uint8_t> color, const ObjType type);
    ~Building();

    void operator=(const Building&) = delete;

    bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size) override;

    virtual void updateFrame(const uint64_t& time) = 0;
    void update(const uint64_t time) override; // Calls update in child class
    void render(Window& window, const Vect<int> renderOffset) override;

    std::string buildingGetSave();
    std::string buildingReadSave(const std::string& save);

    void placeDown(const uint64_t time) override;

protected:
    bool beingBuilt;
    uint64_t timeAtPlace; // Time when the building is placed
    float percentComplete;
    int level; 

    const nlohmann::json& data; // Data for the specific building
};