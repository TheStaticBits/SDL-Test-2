#include "building.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "window.h"
#include "vector.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"
#include "animation.h"

Building::Building(Window& window, const nlohmann::json& data, const ObjType type)
    : Interactable(window, data, {data["size"][0], data["size"][1]}, type), 
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
        if (obj->getType() != Platform_T) continue;
        
        // If the platform is at the bottom of the building
        SDL_Rect objRect = obj->getRect();
        if (objRect.y != pos.y + renderPos.h) continue;

        // If the platform is at least partially in the building
        if (!(objRect.x + objRect.w > pos.x && objRect.x < pos.x + renderPos.w)) continue;

        // Add pixels to the building base that are covered by the platform:

        uint32_t pixelsBeneath = obj->getRect().w;

        // Removing pixels off the side
        if (objRect.x < pos.x)
            pixelsBeneath -= pos.x - objRect.x;
        if (objRect.x + objRect.w > pos.x + renderPos.w)
            pixelsBeneath -= (objRect.x + objRect.w) - (pos.x + renderPos.w);
        
        pChecked += pixelsBeneath;
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

void Building::update(Window& window, const uint64_t& time)
{
    Interactable::update(window, time);

    // Being built timer
    if (beingBuilt)
    {
        uint64_t upgradeTime = getLvlData()["upgradeTime"];
        upgradeTime *= 1000; // Converting to milliseconds

        if (time >= timeAtPlace + upgradeTime)
        {
            beingBuilt = false;
            percentComplete = 0;
            chooseMenu();
        }
        else
            percentComplete = ((float)(time - timeAtPlace) / upgradeTime);
    }
}

void Building::render(Window& window, const Vect<int64_t>& renderOffset)
{
    if (beingBuilt)
    {
        const Vect<int> renderOffsetInt = renderOffset.cast<int>();
        Vect<int64_t> pos = { renderPos.x - renderOffsetInt.x, 
                              renderPos.y - renderOffsetInt.y };

        // Drawing transparent part
        getCurrentAnim().modAlpha(window, ALPHA);
        getCurrentAnim().render(window, pos);

        // Drawing solid part
        SDL_Rect srcRect = getCurrentAnim().getSourceRect();
        SDL_Rect destRect = { pos.xCast<int>(), pos.yCast<int>(),
                              srcRect.w, srcRect.h };
        const Vect<uint32_t> frameSize = getCurrentAnim().getFrame();

        const uint32_t height = static_cast<uint32_t>(percentComplete * srcRect.h);

        // SRC image is not scaled up
        srcRect.y = (frameSize.y - height);
        srcRect.h = height; 
        
        destRect.y += srcRect.y;
        destRect.h = srcRect.h;
        
        getCurrentAnim().modAlpha(window, 255);
        getCurrentAnim().render(window, srcRect, destRect);
    }
    else
        Interactable::render(window, renderOffset);
}

void Building::renderMenu(Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize)
{
    Interactable::renderMenu(window, renderOffset, baseSize);

    if (!hovering) return;
    
    if (currentMenu == "beingBuilt")
    {
        // Progress bar and stuff
    }
}

void Building::chooseMenu()
{
    if (beingBuilt)
    {
        currentMenu = "beingBuilt";
    }
    else
    {
        currentMenu = "home";
    }
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
    const std::vector<std::string> data = util::split(save, ",");

    beingBuilt = std::stoi(data.at(0));
    level =      std::stoi(data.at(1));

    if (beingBuilt) timeAtPlace = std::stoull(data[2]);

    return save.substr(save.find("#") + 1);
}