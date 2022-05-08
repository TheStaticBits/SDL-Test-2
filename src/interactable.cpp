#include "interactable.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"

Interactable::Interactable(Vect<int> size, Vect<int> tileSize, std::vector<Uint8> color, ObjType type)
    : renderPos{0, 0, size.x, size.y}, tileSize(tileSize), renderColor(color), 
    placing(true), placable(true), hovering(false), type(type)
{

}

Interactable::Interactable(ObjType type)
    : placing(false), placable(false), hovering(false), type(type)
{

}

Interactable::~Interactable()
{
    // SDL_DestroyTexture(image);
}

// General canPlace function that every canPlace function should use somewhere
bool Interactable::genCanPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    // If the platform is touching any other object besides itself
    for (std::unique_ptr<Interactable>& obj : objects)
        if (util::collide(renderPos, obj->getRect()) && !obj->isPlacing())
            return false;

    // If the platform is outside of the base boundaries
    if (pos.x < 0 || 
        pos.x + renderPos.w > size.x || 
        pos.y < 0 || 
        pos.y + renderPos.h > size.y)
        return false;

    return true;
}


void Interactable::genRender(Window& window, Vect<int>& renderOffset)
{
    SDL_Rect render = renderPos;
    render.x -= renderOffset.x;
    render.y -= renderOffset.y;

    std::vector<Uint8> color = renderColor;
    if (placing) // Alphafied
    {
        color.push_back(hoveringAlpha);

        if (!placable) // Redified
        {
            color[0] = 255;
            color[1] = 0;
            color[2] = 0;
        }
    }

    window.drawRect(render, color);
}

std::string Interactable::genSaveData()
{
    if (placing) return ""; // Don't save if placing
    std::string save = objTypeNames.at(type) + " ";
    save += std::to_string(renderPos.x) + "," + std::to_string(renderPos.y) + " ";
    save += std::to_string(tileSize.x) + "," + std::to_string(tileSize.y);
    save += "#"; // Divider between general save data and the specific object save data
    return save;
}

std::string Interactable::genReadSave(std::string save)
{
    // Remove name
    save = save.substr(objTypeNames.at(type).size() + 1);
    // Loading save data
    // Basically inverting what is done in the genSaveData function
    std::vector<std::string> data = util::split(save, " ");
    std::vector<std::string> posData = util::split(data[0], ",");
    renderPos.x = std::stoi(posData[0]);
    renderPos.y = std::stoi(posData[1]);
    std::vector<std::string> sizeData = util::split(data[1], ",");
    tileSize.x = std::stoi(sizeData[0]);
    tileSize.y = std::stoi(sizeData[1]);
    renderPos.w = tileSize.x * TILE_SIZE;
    renderPos.h = tileSize.y * TILE_SIZE;

    return save.substr(save.find("#") + 1); // Removing everything before the divider
}