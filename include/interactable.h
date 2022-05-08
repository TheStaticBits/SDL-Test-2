#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"

enum ObjType { PlatformType, BuildingType };
inline std::unordered_map<ObjType, std::string> objTypeNames = {
    { PlatformType, "Platform" },
    { BuildingType, "Building" }
};

// Objects in the home base that can be interacted with
class Interactable
{
public:
    Interactable(Vect<int> size, Vect<int> tileSize, std::vector<Uint8> color, ObjType type);
    Interactable(ObjType type); // For loading from save
    ~Interactable();

    void operator=(const Interactable&) = delete;

    virtual bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData) = 0;
    bool genCanPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size);

    virtual void update(std::time_t seconds) = 0;
    virtual void render(Window& window, Vect<int> renderOffset) = 0;

    std::string genSaveData(); // General save data for the object
    virtual std::string getSave() = 0;
    std::string genReadSave(std::string save);
    virtual void readSave(std::string save) = 0;

    void genRender(Window& window, Vect<int>& renderOffset);

    inline bool isPlacing() const { return placing; }
    inline void setPlacable(bool canPlace) { placable = canPlace; }
    inline Vect<int>& getTileSize() { return tileSize; }
    inline void setPos(Vect<int> pos) { renderPos.x = pos.x; renderPos.y = pos.y; }
    inline void completePlace() { placing = false; }
    inline SDL_Rect& getRect() { return renderPos; }
    inline ObjType& getType() { return type; }
    static inline bool checkSavePart(std::string part, ObjType objType) { return part.substr(0, objTypeNames.at(objType).length()) == objTypeNames.at(objType); }

protected:
    static constexpr int hoveringAlpha = 150;

    // SDL_Texture* image; // Add image later
    SDL_Rect renderPos; // Top left corner
    Vect<int> tileSize; // Amount of tiles it takes up
    std::vector<Uint8> renderColor;

    bool placing;
    bool placable;
    bool hovering;

    ObjType type;
};