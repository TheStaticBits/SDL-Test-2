#include "building.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

Building::Building(const nlohmann::json& data, const Vect<int> tileSize, const std::vector<Uint8> color, const ObjType type)
    : Interactable(tileSize, color, type), 
    beingBuilt(false), updating(false), buildingTimer(0), level(0),
    data(data)
{
    renderPos.w = tileSize.x * TILE_SIZE;
    renderPos.h = tileSize.y * TILE_SIZE;
}

Building::Building(const nlohmann::json& data, const std::vector<Uint8> color, const ObjType type)
    : Interactable(type), renderColor(color), data(data)
{

}

Building::~Building()
{
    
}

bool Building::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    if (!Interactable::genCanPlace(pos, objects, size)) return false;
    if (pos.y + renderPos.h == size.y) return true; // Bottom of map

    int pChecked = 0; // Tiles of the building base with a platform below

    for (std::unique_ptr<Interactable>& obj : objects)
    {
        if (obj->getType() == Platform_T)
        {
            SDL_Rect objRect = obj->getRect();
            // If the platform is at the bottom of the building
            if (objRect.y == pos.y + renderPos.h)
            {
                // If the platform is at least partially in the building
                if (objRect.x + objRect.w > pos.x && objRect.x < pos.x + renderPos.w)
                {
                    int pixelsBeneath = obj->getRect().w;

                    // Removing pixels off the side
                    if (objRect.x < pos.x)
                        pixelsBeneath -= pos.x - objRect.x;
                    if (objRect.x + objRect.w > pos.x + renderPos.w)
                        pixelsBeneath -= (objRect.x + objRect.w) - (pos.x + renderPos.w);
                    
                    pChecked += pixelsBeneath;
                }
            }
        }
    }
    
    return pChecked == renderPos.w;
}

void Building::update(std::time_t seconds)
{
    // Handling being built
    if (beingBuilt)
    {
        if (buildingTimer > 0)
            buildingTimer--;
        else
            beingBuilt = false;
    }
    else
    {
        // Functionality
    }

    updateBuilding(seconds);
}

void Building::render(Window& window, Vect<int> renderOffset)
{
    // more code here?? perhaps
    Interactable::genRender(window, renderOffset);
}

std::string Building::buildingGetSave()
{
    std::string save = Interactable::genSaveData();

    save += std::to_string(beingBuilt)    + "," + 
            std::to_string(updating)      + "," + 
            std::to_string(buildingTimer) + "," + 
            std::to_string(level)         + "#"; // Divider

    return save;
}

std::string Building::buildingReadSave(const std::string save)
{
    std::vector<std::string> data = util::split(save, ",");

    beingBuilt =    std::stoi(data[0]);
    updating =      std::stoi(data[1]);
    buildingTimer = std::stoi(data[2]);
    level =         std::stoi(data[3]);

    return save.substr(save.find("#") + 1);
}

void Building::placeDown()
{

}