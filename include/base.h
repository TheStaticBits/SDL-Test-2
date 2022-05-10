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

inline const int TILE_SIZE = 20;
inline const std::string RemoveObj = "Remove";

class Base
{
public:
    Base();
    ~Base();

    void operator=(const Base&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool>& keys,
                std::unordered_map<Uint8, bool>& mouseButtons, 
                Vect<int>& mousePos,
                Vect<int>& renderOffset);

    void render(Window& window, Vect<int> renderOffset);

    inline Vect<int> getSize() const { return size; }
    inline std::vector<std::unique_ptr<Interactable>>& getObjects() { return objects; }

    std::string getSave();
    void readSave(std::string save);
    inline bool checkSavePart(std::string save) { return save.substr(0, saveName.size()) == saveName; }


private:
    inline static const std::string saveName = "Base";

    static constexpr char* bDataPath = (char*)"data/buildings.json";
    nlohmann::json buildingData;

    Vect<int> size;

    std::vector<std::unique_ptr<Interactable>> objects;

    bool placing;
};