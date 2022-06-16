#include "platform.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

Platform::Platform(Window& window, const nlohmann::json& data, const uint32_t tileWidth)
    : Interactable(window, data, {tileWidth, 1}, Platform_T),
      data(data)
{
    initParts();
}

Platform::Platform(Window& window, const nlohmann::json& data, std::string save)
    : Interactable(window, data, Platform_T),
      data(data)
{
    modColor = {0, 255, 0};

    save = Interactable::readSave(save);
    readSave(save);

    initParts();
}

Platform::~Platform()
{

}

void Platform::initParts()
{
    Vect<int> frameSize = (getCurrentAnim()->getFrame()).cast<int>();

    head = { static_cast<int>(data["headX"][0].get<int>() * SCALE), 0,
             static_cast<int>(data["headX"][1].get<int>() * SCALE - data["headX"][0].get<int>() * SCALE), frameSize.y };
    body = { static_cast<int>(data["bodyX"][0].get<int>() * SCALE), 0,
             static_cast<int>(data["bodyX"][1].get<int>() * SCALE - data["bodyX"][0].get<int>() * SCALE), frameSize.y };
}

bool Platform::canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size)
{
    return Interactable::canPlace(pos, objects, size);
}

void Platform::render(Window& window, const Vect<int64_t>& renderOffset)
{
    Interactable::setModColor(window);

    const Vect<int> renderOffsetInt = renderOffset.cast<int>();
    const int frameOffset = static_cast<int>(getCurrentAnim()->getFrameNum() * TILE_SIZE);

    head.x = static_cast<int>(data["headX"][0].get<int>() * SCALE) + frameOffset;
    body.x = static_cast<int>(data["bodyX"][0].get<int>() * SCALE) + frameOffset;
    
    for (uint32_t tileX = 0; tileX < tileSize.x; tileX++ )
    {
        SDL_Rect curRect;
        if (tileX == 0 || tileX == tileSize.x - 1) curRect = head;
        else curRect = body;

        // Finds position centered on the current tile for the image
        SDL_Rect renderRect = { (renderPos.x - renderOffsetInt.x + 
                                 static_cast<int>((tileX * TILE_SIZE) + 
                                 (TILE_SIZE / 2) - (curRect.w / 2))),

                                (renderPos.y - renderOffsetInt.y + 
                                 static_cast<int>((TILE_SIZE / 2) - (curRect.h / 2))),

                                curRect.w, curRect.h};
        
        window.render(getCurrentAnim()->getTexture(), curRect, renderRect, 
                      (tileX == tileSize.x - 1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
    }
}

std::string Platform::getSave()
{
    std::string save = Interactable::getSave();

    save += std::to_string(tileSize.x);

    return save;
}

std::string Platform::readSave(std::string& save)
{
    tileSize.y = 1; // Height of platform is always 1
    tileSize.x = std::stoi(save);

    renderPos.w = tileSize.x * TILE_SIZE;
    renderPos.h = tileSize.y * TILE_SIZE;

    return ""; // No more save to read
}