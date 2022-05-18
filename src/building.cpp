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

bool Building::canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size)
{
    if (!Interactable::canPlace(pos, objects, size)) return false;
    if (pos.y + renderPos.h == size.y) return true; // Bottom of map

    uint32_t pChecked = 0; // Tiles of the building base with a platform below

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
                    uint32_t pixelsBeneath = obj->getRect().w;

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
    
    return pChecked == static_cast<uint32_t>(renderPos.w);
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
        uint64_t upgradeTime = data[std::to_string(level)]["upgradeTime"];
        upgradeTime *= 1000; // Converting to milliseconds

        if (time >= timeAtPlace + upgradeTime)
        {
            beingBuilt = false;
            percentComplete = 100;
        }
        else
        {
            percentComplete = 1 - ((float)(time - timeAtPlace) / upgradeTime);
        }
    }
}

void Building::render(Window& window, const Vect<int64_t>& renderOffset)
{
    if (beingBuilt)
    {
        Vect<int> renderOffsetInt = renderOffset.cast<int>();
        SDL_Rect renderRect = {
            renderPos.x - renderOffsetInt.x, renderPos.y - renderOffsetInt.y,
            renderPos.w, renderPos.h
        };

        // Drawing transparent part
        std::vector<uint8_t> color = renderColor;
        color.push_back(alpha);
        window.drawRect(renderRect, color);

        // Drawing solid part
        const uint32_t solidHeight = static_cast<uint32_t>(percentComplete * renderPos.h);
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