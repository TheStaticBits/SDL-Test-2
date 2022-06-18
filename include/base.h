#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

class Player;
#include "window.h"
#include "vector.h"
#include "interactable.h"
#include "particle.h"

inline const uint16_t TILE_SIZE = 20 * SCALE;
inline const std::string RemoveObj = "Remove"; // Used for not saving objects that have not been placed

// The second in the pair is the distance between 
// each particle created for that layer. 
inline const std::vector<std::pair<ParticleData, uint32_t>> BG_PARTICLE_DATA = {
    //                   rotation  movement
    //      Color         speed,    speed   size   parallax   distance
    { { { 0, 120, 120 },  6.0f,     5.0f,   50,    3.0f },    100 },
    { { { 0, 140, 140 },  7.0f,     10.0f,  60,    2.5f },    140 },
    { { { 0, 170, 185 },  8.0f,     15.0f,  80,    2.0f },    180 },
    { { { 0, 200, 220 },  9.0f,     20.0f,  90,    1.5f },    200 }
};

class Base
{
public:
    Base(Window& window);
    ~Base();

    void operator=(const Base&) = delete;

    void update(Window& window, const Vect<int64_t>& renderOffset);

    void renderMinimap(Window& window, Player& player);
    void renderTiles  (Window& window, const Vect<int64_t> renderOffset);
    void renderBg     (Window& window, const Vect<int64_t> renderOffset);
    void renderMenues (Window& window, const Vect<int64_t> renderOffset);

    void resetBuildingTextures(Window& window);

    inline Vect<uint32_t> getSize() const { return size; }
    inline std::vector<std::unique_ptr<Interactable>>& getObjects() { return objects; }

    std::string getSave();
    void readSave(std::string save, Window& window);
    inline bool checkSavePart(std::string save) { return save.substr(0, SAVE_NAME.length()) == SAVE_NAME; }

private:
    const nlohmann::json& getBData(const ObjType type) { return buildingData[objTNames.at(type)]; } 

    void updatePlacingControls(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate);
    void updateBuildings(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate);
    void updatePlacing(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate);

    void initParticles(Window& window);
    void updateParticles(Window& window, const Vect<int64_t>& renderOffset);

    inline static const std::string SAVE_NAME = "Base";

    inline static const uint32_t minBgParticleAngle = 310;
    inline static const uint32_t maxBgParticleAngle = 320; // Up to, not including 

    static constexpr char* B_DATA_PATH = (char*)"res/data/buildings.json";
    nlohmann::json buildingData;
    
    static constexpr char* P_IMG_PATH = (char*)"res/graphics/bgParticle.png";
    SDL_Texture* bgParticleTex;
    std::vector<Particle> bgParticles;

    Vect<uint32_t> size;

    std::vector<std::unique_ptr<Interactable>> objects;
    std::unordered_map<ObjType, uint64_t> count; // Amount of each building
    
    bool placing;

    inline static const Vect<uint32_t> minimapSize = { 75 * SCALE , 50 * SCALE };
    inline static const uint8_t minimapAlpha = 150;
    SDL_Texture* minimap;
};