#include "building.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

Building::Building(const nlohmann::json& data, const std::vector<uint8_t> color, const ObjType type)
    : Interactable({data["size"][0], data["size"][1]}, color, type), 
      beingBuilt(false), timeAtPlace(0), percentComplete(0),
      level(1), data(data)
{
    
}

Building::~Building()
{
    
}

bool Building::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    if (!Interactable::canPlace(pos, objects, size)) return false;
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

void Building::completePlace(const uint64_t& time)
{
    Interactable::completePlace(time);
    
    beingBuilt = true;
    timeAtPlace = time;
    percentComplete = 0;
}

void Building::update(const uint64_t& time)
{
    // Being built timer
    if (beingBuilt)
    {
        int upgradeTime = data[std::to_string(level)]["upgradeTime"];
        upgradeTime *= 1000; // Converting to milliseconds

        if ((int)time >= (int)timeAtPlace + upgradeTime)
        {
            beingBuilt = false;
            percentComplete = 100;
        }
        else
        {
            percentComplete = 1 - ((float)(time - timeAtPlace) / upgradeTime);
        }
    }

    updateFrame(time);
}

void Building::render(Window& window, const Vect<int>& renderOffset)
{
    if (beingBuilt)
    {
        SDL_Rect renderRect = {
            renderPos.x - renderOffset.x, renderPos.y - renderOffset.y,
            renderPos.w, renderPos.h
        };

        // Drawing transparent part
        std::vector<uint8_t> color = renderColor;
        color.push_back(alpha);
        window.drawRect(renderRect, color);

        // Drawing solid part
        const int solidHeight = static_cast<int>(percentComplete * renderPos.h);
        renderRect.y += solidHeight;
        renderRect.h -= solidHeight;
        window.drawRect(renderRect, renderColor);
    }
    else
        Interactable::render(window, renderOffset);
}

std::string Building::getSave()
{
    std::string save = "";

    save += std::to_string(beingBuilt) + "," +
            std::to_string(level); 

    // Only saving timeAtPlace if it's still being built 
    if (beingBuilt)
        save += "," + std::to_string(timeAtPlace) + "#";
    else 
        save += "#"; // Divider
        
    return save;
}

std::string Building::readSave(std::string& save)
{
    std::vector<std::string> data = util::split(save, ",");

    beingBuilt = std::stoi(data[0]);
    level =      std::stoi(data[1]);

    if (beingBuilt) timeAtPlace = std::stoull(data[2]);

    return save.substr(save.find("#") + 1);
}