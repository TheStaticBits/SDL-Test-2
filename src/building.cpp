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

Building::Building(Vect<int> tileSize, std::vector<Uint8>& color)
    : Interactable({tileSize.x / TILE_SIZE, tileSize.y / TILE_SIZE}, tileSize, color, BuildingType), buildingTimer(0)
{

}

Building::~Building()
{
    
}

bool Building::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData)
{
    if (genCanPlace(pos, objects, size)) return false;

    int pChecked = 0; // Tiles of the building base with a platform below

    for (std::unique_ptr<Interactable>& obj : objects)
    {
        if (obj->getType() == PlatformType)
        {
            // If the platform is at the bottom of the building
            if (obj->getRect().y == renderPos.y + renderPos.h)
            {
                int pixelsBeneath = obj->getRect().w

                // Removing pixels off the side
                if (obj->getRect().x < renderPos.x)
                    pixelsBeneath -= renderPos.x - obj->getRect().x;
                if (obj->getRect().x > renderPos.x)
                    pixelsBeneath -= obj->getRect().x - renderPos.x;
                
                pChecked += pixelsBeneath;
            }
        }
    }
    
    return pChecked == renderPos.w;
}

void Building::update()
{
    // timer updating and whatnot
}

void Building::render(Window& window, Vect<int> renderOffset)
{
    // more code here?? perhaps
    Interactable::genRender(window, renderOffset);
}