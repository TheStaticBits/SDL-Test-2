#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "interactable.h"
#include "platform.h"
#include "building.h"
#include "particle.h"

inline const uint16_t TILE_SIZE = 20;
inline const std::string RemoveObj = "Remove"; // Used for not saving objects that have not been placed

inline const std::vector<ParticleData> bgParticleData = {
    // Layer 0: biggest and closest
    { { 0, 240, 240 }, 5.0f, 2000.0f, 10.0f, 2.0f}, // L0 
};

class Base
{
public:
    Base(Window& window);
    ~Base();

    void operator=(const Base&) = delete;

    void update(Window& window, const Vect<int64_t>& renderOffset);

    void renderMinimap(Window& window);
    void renderTiles  (Window& window, const Vect<int64_t> renderOffset);
    void renderBg     (Window& window, const Vect<int64_t> renderOffset);
    void renderMenues (Window& window, const Vect<int64_t> renderOffset);

    inline Vect<uint32_t> getSize() const { return size; }
    inline std::vector<std::unique_ptr<Interactable>>& getObjects() { return objects; }

    std::string getSave();
    void readSave(std::string save);
    inline bool checkSavePart(std::string save) { return save.substr(0, SAVE_NAME.length()) == SAVE_NAME; }

private:
    inline static const std::string SAVE_NAME = "Base";

    static constexpr char* B_DATA_PATH = (char*)"res/data/buildings.json";
    nlohmann::json buildingData;
    
    static constexpr char* P_IMG_PATH = (char*)"res/graphics/bgParticle.png";
    SDL_Texture* bgParticleTex;
    std::vector<Particle> bgParticles;

    Vect<uint32_t> size;

    std::vector<std::unique_ptr<Interactable>> objects;
    std::unordered_map<ObjType, uint64_t> count; // Amount of each building
    
    bool placing;

    inline static const float minimapScale = 0.1f;
    SDL_Texture* minimap;
};