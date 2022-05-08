#include "building.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "base.h"
#include "interactable.h"

Building::Building(Vect<int> tileSize, std::vector<Uint8> color)
    : Interactable(tileSize * TILE_SIZE, tileSize, color, BuildingType), buildingTimer(0)
{

}

Building::Building(std::string save)
    : Interactable(BuildingType), beingBuilt(false), buildingTimer(0)
{
    save = genReadSave(save);
    readSave(save);
}

Building::~Building()
{
    
}

bool Building::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData)
{
    if (!Interactable::genCanPlace(pos, objects, size)) return false;
    if (pos.y + renderPos.h == size.y) return true; // Bottom of map

    int pChecked = 0; // Tiles of the building base with a platform below

    for (std::unique_ptr<Interactable>& obj : objects)
    {
        if (obj->getType() == PlatformType)
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
    // timer updating and whatnot
}

void Building::render(Window& window, Vect<int> renderOffset)
{
    // more code here?? perhaps
    Interactable::genRender(window, renderOffset);
}

std::string Building::getSave()
{
    std::string save = Interactable::genSaveData();

    return save;
}

void Building::readSave(std::string save)
{
    
}