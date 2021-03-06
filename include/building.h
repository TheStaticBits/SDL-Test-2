#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

template <typename T>
struct Vect;

class Window;
class Base;
#include "interactable.h"

// Contains more specific code, for each building
// Buildings will inherit from this class
class Building : public Interactable
{
public:
    Building(Window& window, const nlohmann::json& data, const ObjType type);
    ~Building();

    void operator=(const Building&) = delete;

    bool canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size) override;
    void completePlace(const uint64_t& time) override;

    void update(Window& window, const uint64_t& time) override;
    void render(Window& window, const Vect<int64_t>& renderOffset) override;
    void renderMenu(Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize) override;
    
    void chooseMenu() override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;
    
    inline const uint32_t getMax(const uint32_t coreLvl) const { return data["max"]["Core L" + std::to_string(coreLvl)]; }
    inline const nlohmann::json& getLvlData() { return data[std::to_string(level)]; }

protected:
    bool beingBuilt;
    uint64_t timeAtPlace; // Time when the building is placed
    float percentComplete;
    uint32_t level; 

    const nlohmann::json& data; // Data for the specific building
};