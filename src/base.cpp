#include "base.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <memory>
#include <stdlib.h>

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
#include "particle.h"
#include "player.h"

Base::Base(Window& window)
    : buildingData(nlohmann::json::parse(std::ifstream(B_DATA_PATH))), 
      bgParticleTex(window.loadTexture(P_IMG_PATH)),
      size{TILE_SIZE * 34, TILE_SIZE * 24}, // TEMPORARY, will change in the future
      placing(false),
      minimap(window.createTex(size.x, size.y))
{
    initParticles(window);
    
    // Initializing some static variables
    Button::loadButtonData(window);
    Interactable::loadMenuData();
}

Base::~Base()
{
    SDL_DestroyTexture(bgParticleTex);
}

void Base::update(Window& window, const Vect<int64_t>& renderOffset)
{
    uint64_t timeAtUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // First one temporary
    updatePlacingControls(window, renderOffset, timeAtUpdate);
    updateBuildings(window, renderOffset, timeAtUpdate);
    updatePlacing(window, renderOffset, timeAtUpdate);

    // Updating bgParticles
    updateParticles(window, renderOffset);
}

// TEMPORARY (will add shop for placing later)
void Base::updatePlacingControls(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate)
{
    if (!placing)
    {
        // Temporary
        if (window.pKey(SDLK_p)) // Platform
        {
            placing = true;
            count[Platform_T]++;
            objects.push_back(std::make_unique<Platform>(window, getBData(Platform_T), 4));
        } 
        if (window.pKey(SDLK_b)) // Building
        {
            placing = true;
            count[SilverStorage_T]++;
            objects.push_back(std::make_unique<SilverStorage>(window, getBData(SilverStorage_T)));
        }

        // Clear any menues open
        if (placing)
            for (std::unique_ptr<Interactable>& obj : objects)
                obj->removeMenu();
    }
}

void Base::updateBuildings(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate)
{
    // Updating the menu that is open before others
    for (std::unique_ptr<Interactable>& obj : objects)
        if (obj->menuOpen())
            obj->checkMenu(window, renderOffset);

    for (std::unique_ptr<Interactable>& obj : objects)
    {
        obj->update(window, timeAtUpdate);

        if (!obj->menuOpen())
            obj->checkMenu(window, renderOffset);
    }
}

void Base::updatePlacing(Window& window, const Vect<int64_t>& renderOffset, const uint64_t timeAtUpdate)
{
    if (placing)
    {
        const Vect<int64_t> mousePos = window.getCamMousePos();
        
        // Getting the tile the mouse is hovering over
        Vect<int32_t> placeTile = ((mousePos + renderOffset) / TILE_SIZE).cast<int32_t>();

        // Changing floor to ceil for negatives
        if (mousePos.x + renderOffset.x < 0)
            placeTile.x -= 1;
        if (mousePos.y + renderOffset.y < 0)
            placeTile.y -= 1;

        for (std::unique_ptr<Interactable>& obj : objects)
        {
            if (obj->isPlacing())
            {
                placeTile -= (obj->getTileSize() / 2).cast<int32_t>();

                Vect<int64_t> screenPos = placeTile.cast<int64_t>() * TILE_SIZE;
                obj->setPos(screenPos);

                if (obj->canPlace(screenPos, objects, size))
                {
                    obj->setPlacable(true);

                    if (window.button(SDL_BUTTON_LEFT))
                    {
                        placing = false;
                        obj->completePlace(timeAtUpdate);
                    }
                }
                else
                    obj->setPlacable(false);
            }
        }
    }
}

void Base::initParticles(Window& window)
{
    const Vect<uint32_t> winSize = window.getSize();
    const Vect<int32_t> maxRenderOffset = (size - winSize).cast<int32_t>();
    Vect<int64_t> spawnPos;
    
    for (const std::pair<ParticleData, uint32_t>& layer : BG_PARTICLE_DATA)
    {
        for (spawnPos.x = 0; spawnPos.x <= winSize.x + (maxRenderOffset.x / layer.first.parallax); spawnPos.x += layer.second)
        {
            for (spawnPos.y = 0; spawnPos.y <= winSize.y +(maxRenderOffset.y / layer.first.parallax); spawnPos.y += layer.second)
            {
                // Chooses random angle between min and max angles
                float moveAngle = static_cast<float>((rand() % (maxBgParticleAngle - minBgParticleAngle)) + minBgParticleAngle);
                
                float startAngle = static_cast<float>(rand() % 365);

                bgParticles.push_back(Particle(bgParticleTex, spawnPos.cast<float>(), startAngle, moveAngle, layer.first));
            }
        }
    }
}

void Base::updateParticles(Window& window, const Vect<int64_t>& renderOffset)
{
    for (Particle& p : bgParticles)
        p.update(window, size);
}

void Base::renderMinimap(Window& window, Player& player)
{
    window.setTarget(minimap);

    const Vect<int> sizeInt = size.cast<int>();
    SDL_Rect background = { 0, 0, sizeInt.x, sizeInt.y };
    
    window.drawRect(background, {0, 0, 255, 255});

    renderTiles(window, Vect<int64_t>(0, 0));
    player.render(window, Vect<int64_t>(0, 0));

    window.resetTarget();

    window.modAlpha(minimap, minimapAlpha);

    // Drawing minimap to screen
    
    SDL_Rect dest = { 5, static_cast<int>(window.getSize().y - minimapSize.y - 5),
                      minimapSize.xCast<int>(), minimapSize.yCast<int>() };
    window.render(minimap, dest);
}

void Base::renderTiles(Window& window, const Vect<int64_t> renderOffset)
{
    for (std::unique_ptr<Interactable>& obj : objects)
        obj->render(window, renderOffset);
}

void Base::renderBg(Window& window, const Vect<int64_t> renderOffset)
{
    const Vect<int> sizeInt = size.cast<int>();
    SDL_Rect background = { 0, 0, sizeInt.x, sizeInt.y };
    
    window.drawRect(background, {0, 100, 100, 255});

    for (Particle& particle : bgParticles)
        particle.render(window, renderOffset);
}

void Base::renderMenues(Window& window, const Vect<int64_t> renderOffset)
{
    // Render any menues on top of everything
    if (!placing)
        for (std::unique_ptr<Interactable>& obj : objects)
            obj->renderMenu(window, renderOffset, size);
}

void Base::resetTextures(Window& window)
{
    Interactable::resetTextures(window, buildingData);
    Button::resetTextures(window);
}

std::string Base::getSave()
{
    std::string save = SAVE_NAME;

    // Add money related stuff here

    std::string saveSection;
    for (std::unique_ptr<Interactable>& obj : objects)
    {
        saveSection = obj->getSave() + "|";
        // If the object is to be not saved (may be being placed)
        if (saveSection.substr(0, RemoveObj.length()) != RemoveObj)
            save += saveSection;
    }
    
    if (save.length() > SAVE_NAME.length())
        save = save.substr(0, save.length() - 1); // Removing last divider
    
    return save;
}

void Base::readSave(std::string save, Window& window)
{
    if (save.length() > SAVE_NAME.length())
    {
        save = save.substr(SAVE_NAME.length());
        std::vector<std::string> saveList = util::split(save, "|");

        for (std::string& obj : saveList)
        {
            if (objCheckSavePart(obj, Platform_T))
            {
                count[Platform_T]++;
                objects.push_back(std::make_unique<Platform>(window, getBData(Platform_T), obj));
            }
            else if (objCheckSavePart(obj, SilverStorage_T))
            {
                count[SilverStorage_T]++;
                objects.push_back(std::make_unique<SilverStorage>(window, getBData(SilverStorage_T), obj));
            }
        }
    }
}