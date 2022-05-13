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
      beingBuilt(false), timeAtPlace(0), percentComplete(100),
      level(1), data(data)
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

void Building::update(const std::time_t time)
{
    // Being built timer
    if (beingBuilt)
    {
        int upgradeTime = data[std::to_string(level)]["upgradeTime"];

        if ((int)time >= (int)timeAtPlace + upgradeTime)
        {
            beingBuilt = false;
            percentComplete = 100;
        }
        else
        {
            percentComplete = (float)(time - timeAtPlace) / upgradeTime;
        }
    }

    updateFrame(time);
}

void Building::render(Window& window, Vect<int> renderOffset)
{
    if (beingBuilt)
    {
        // Drawing transparent part
        std::vector<Uint8> color = renderColor;
        color.push_back(alpha);
        window.drawRect(renderPos, color);

        // Drawing solid part
        const int solidHeight = static_cast<int>(percentComplete * renderPos.h);
        SDL_Rect renderRect = {
            renderPos.x, renderPos.y + solidHeight,
            renderPos.w, renderPos.h - solidHeight
        }; 
        window.drawRect(renderRect, renderColor);
    }
    else
        Interactable::genRender(window, renderOffset);
}

std::string Building::buildingGetSave()
{
    std::string save = Interactable::genGetSave();

    save += std::to_string(beingBuilt)    + "," + 
            std::to_string(timeAtPlace)   + "," + 
            std::to_string(level)         + "#"; // Divider

    return save;
}

std::string Building::buildingReadSave(const std::string& save)
{
    std::vector<std::string> data = util::split(save, ",");

    beingBuilt =  std::stoi(data[0]);
    timeAtPlace = std::stoi(data[1]);
    level =       std::stoi(data[2]);

    return save.substr(save.find("#") + 1);
}

void Building::placeDown(const std::time_t time)
{
    beingBuilt = true;
    timeAtPlace = time;
    percentComplete = 0;
}