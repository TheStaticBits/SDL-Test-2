#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"

enum ObjType { Platform_T, SilverStorage_T};
inline std::unordered_map<ObjType, std::string> objTNames = {
    { Platform_T,      "Platform"  },
    { SilverStorage_T, "SilverStor"}
};

// Objects in the home base that can be interacted with
class Interactable
{
public:
    Interactable(const Vect<int> tileSize, const std::vector<uint8_t> color, const ObjType type);
    Interactable(const ObjType type); // For loading from save
    virtual ~Interactable();

    void operator=(const Interactable&) = delete;

    virtual bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size) = 0;
    bool genCanPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size);

    virtual void update(const uint64_t seconds) = 0;
    virtual void render(Window& window, const Vect<int> renderOffset) = 0;
    virtual void placeDown(const uint64_t time) = 0; // Called when placed down

    std::string genGetSave(); // General save data for the object
    virtual std::string getSave() = 0;
    std::string genReadSave(std::string save);
    virtual void readSave(const std::string& save) = 0;

    void genRender(Window& window, const Vect<int>& renderOffset);

    inline bool isPlacing() const { return placing; }
    inline void setPlacable(bool canPlace) { placable = canPlace; }
    inline Vect<int>& getTileSize() { return tileSize; }
    inline void setPos(Vect<int> pos) { renderPos.x = pos.x; renderPos.y = pos.y; }
    inline void completePlace(const uint64_t time) { placing = false; placeDown(time); }
    inline SDL_Rect& getRect() { return renderPos; }
    inline const ObjType& getType() { return type; }

    static inline bool checkSavePart(const std::string part, const ObjType objType) 
    { 
        return part.substr(0, objTNames.at(objType).length()) == objTNames.at(objType); 
    }

protected:
    static constexpr int alpha = 150;

    // SDL_Texture* image; // Add image later
    Vect<int> tileSize; // Amount of tiles it takes up
    std::vector<uint8_t> renderColor;
    SDL_Rect renderPos; // Top left corner

    bool placing;
    bool placable;
    bool hovering;

    const ObjType type;
};