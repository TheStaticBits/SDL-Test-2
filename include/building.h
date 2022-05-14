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

    // Called in update function for child class update functionality
    virtual void updateFrame(const uint64_t& time) { };

    void update(const uint64_t& time) override; // Calls update in child class
    void render(Window& window, const Vect<int>& renderOffset) override;
    
    void placeDown(const uint64_t& time) override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;

protected:
    bool beingBuilt;
    uint64_t timeAtPlace; // Time when the building is placed
    float percentComplete;
    int level; 

    const nlohmann::json& data; // Data for the specific building
};