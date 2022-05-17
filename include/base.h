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
#include "shop.h"

inline const uint16_t TILE_SIZE = 20;
inline const std::string RemoveObj = "Remove";

class Base
{
public:
    Base(Window& window);
    ~Base();

    void operator=(const Base&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool>& keys,
                std::unordered_map<uint8_t, bool>& mouseButtons, 
                Vect<int64_t>& mousePos,
                Vect<int64_t>& renderOffset);

    void render(Window& window, Vect<int64_t> renderOffset);

    inline Vect<uint32_t> getSize() const { return size; }
    inline std::vector<std::unique_ptr<Interactable>>& getObjects() { return objects; }

    std::string getSave();
    void readSave(std::string save);
    inline bool checkSavePart(std::string save) { return save.substr(0, SAVE_NAME.length()) == SAVE_NAME; }


private:
    inline static const std::string SAVE_NAME = "Base";

    static constexpr char* B_DATA_PATH = (char*)"res/buildings.json";
    nlohmann::json buildingData;

    Vect<uint32_t> size;

    std::vector<std::unique_ptr<Interactable>> objects;
    std::unordered_map<ObjType, uint64_t> count;
    Shop shop;

    bool placing;
};