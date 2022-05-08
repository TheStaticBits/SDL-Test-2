#include "base.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
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

Base::Base()
    : buildingData(nlohmann::json::parse(std::ifstream(bDataPath))), 
      size{R_WIN_WIDTH, R_WIN_HEIGHT}, placing(false)
{

}

Base::~Base()
{

}

void Base::update(std::unordered_map<SDL_Keycode, bool>& keys,
                  std::unordered_map<Uint8, bool>& mouseButtons, 
                  Vect<int>& mousePos,
                  Vect<int>& renderOffset)
{
    if (!placing)
    {
        // Temporary
        if (keys[SDLK_SPACE])
        {
            placing = true;
            objects.push_back(std::make_unique<Platform>(4));
        } 
        if (mouseButtons[SDL_BUTTON_RIGHT])
        {
            placing = true;
            objects.push_back(std::make_unique<Building>(Vect<int>{2, 3}, std::vector<Uint8>{0, 255, 255}));
        }
    }

    for (std::unique_ptr<Interactable>& obj : objects)
        obj->update(std::time(nullptr));

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

                if (obj->canPlace(screenPos, objects, size, buildingData))
                {
                    obj->setPlacable(true);

                    if (mouseButtons[SDL_BUTTON_LEFT])
                    {
                        placing = false;
                        obj->completePlace();
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
    std::string save = saveName + " ";

    // Add money related stuff here

    for (std::unique_ptr<Interactable>& obj : objects)
        save += obj->getSave() + " | ";
    
    if (save.size() > saveName.length() + 1)
        save = save.substr(0, save.length() - 3); // Removing last divider
    
    return save;
}

void Base::readSave(std::string save)
{
    if (save.size() > saveName.length() + 1)
    {
        save = save.substr(saveName.length() + 1);
        std::vector<std::string> saveList = util::split(save, " | ");

        for (std::string& obj : saveList)
        {
            if (Interactable::checkSavePart(obj, PlatformType))
                objects.push_back(std::make_unique<Platform>(obj));
            else if (Interactable::checkSavePart(obj, BuildingType))
                objects.push_back(std::make_unique<Building>(obj, std::vector<Uint8>{0, 255, 255}));
        }
    }
}