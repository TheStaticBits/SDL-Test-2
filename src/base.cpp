#include "base.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "interactable.h"
#include "platform.h"
#include "building.h"
#include "utility.h"
#include "silverStorage.h"

Base::Base()
    : buildingData(nlohmann::json::parse(std::ifstream(bDataPath))), 
      size{R_WIN_WIDTH, R_WIN_HEIGHT}, placing(false)
{

}

Base::~Base()
{

}

void Base::update(std::unordered_map<SDL_Keycode, bool>& keys,
                  std::unordered_map<uint8_t, bool>& mouseButtons, 
                  Vect<int>& mousePos,
                  Vect<int>& renderOffset)
{
    uint64_t timeAtUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    if (!placing)
    {
        // Temporary
        if (keys[SDLK_p]) // Platform
        {
            placing = true;
            objects.push_back(std::make_unique<Platform>(4));
        } 
        if (keys[SDLK_b]) // Building
        {
            placing = true;
            objects.push_back(std::make_unique<SilverStorage>(buildingData[objTNames[SilverStorage_T]]));
        }
    }

    for (std::unique_ptr<Interactable>& obj : objects)
        obj->update(timeAtUpdate);

    if (placing)
    {
        // Getting the tile the mouse is hovering over
        Vect<int> placeTile = (mousePos + renderOffset) / TILE_SIZE;

        // Changing floor to ceil for negatives
        if (mousePos.x + renderOffset.x < 0)
            placeTile.x -= 1;
        if (mousePos.y + renderOffset.y < 0)
            placeTile.y -= 1;

        for (std::unique_ptr<Interactable>& obj : objects)
        {
            if (obj->isPlacing())
            {
                placeTile -= obj->getTileSize() / 2;

                Vect<int> screenPos = placeTile * TILE_SIZE;
                obj->setPos(screenPos);

                if (obj->canPlace(screenPos, objects, size))
                {
                    obj->setPlacable(true);

                    if (mouseButtons[SDL_BUTTON_LEFT])
                    {
                        placing = false;
                        obj->completePlace(timeAtUpdate);
                        std::cout << "complete" << std::endl;
                    }
                }
                else
                    obj->setPlacable(false);
            }
        }
    }
}

void Base::render(Window& window, Vect<int> renderOffset)
{
    SDL_Rect background = {-renderOffset.x, -renderOffset.y, size.x, size.y};
    window.drawRect(background, {0, 0, 255, 255});

    for (std::unique_ptr<Interactable>& obj : objects)
        obj->render(window, renderOffset);
}

std::string Base::getSave()
{
    std::string save = saveName;

    // Add money related stuff here

    std::string saveSection;
    for (std::unique_ptr<Interactable>& obj : objects)
    {
        saveSection = obj->getSave() + "|";
        // If the object is to be not saved (may be being placed)
        if (saveSection.substr(0, RemoveObj.length()) != RemoveObj)
            save += saveSection;
    }
    
    if (save.size() > saveName.length())
        save = save.substr(0, save.length() - 1); // Removing last divider
    
    return save;
}

void Base::readSave(std::string save)
{
    if (save.size() > saveName.length())
    {
        save = save.substr(saveName.length());
        std::vector<std::string> saveList = util::split(save, "|");

        for (std::string& obj : saveList)
        {
            if (objCheckSavePart(obj, Platform_T))
                objects.push_back(std::make_unique<Platform>(obj));
            else if (objCheckSavePart(obj, SilverStorage_T))
                objects.push_back(std::make_unique<SilverStorage>(buildingData[objTNames[SilverStorage_T]], obj));
        }
    }
}