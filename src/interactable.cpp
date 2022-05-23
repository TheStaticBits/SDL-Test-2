#include "interactable.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"

Interactable::Interactable(const Vect<uint32_t> tileSize, const std::vector<uint8_t> color, const ObjType type)
    : tileSize(tileSize), renderColor(color),
      renderPos{ 0, 0, static_cast<int>(tileSize.x * TILE_SIZE), 
                       static_cast<int>(tileSize.y * TILE_SIZE) }, 
      placing(true), placable(true), hovering(false), clicked(false),
      type(type)
{

}

Interactable::Interactable(const ObjType type)
    : placing(false), placable(false), hovering(false), clicked(false),
      type(type)
{

}

Interactable::~Interactable()
{
    // SDL_DestroyTexture(image);
}

bool Interactable::canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size)
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

void Interactable::completePlace(const uint64_t& time)
{
    placing = false; 
}

void Interactable::checkMenu(const Vect<int64_t>& mousePos,
                             std::unordered_map<uint8_t, bool>& mouseButtons, 
                             const Vect<int64_t>& renderOffset)
{

    if (!placing)
    {
        if (util::collide(renderPos, mousePos + renderOffset))
        {
            if (mouseButtons[SDL_BUTTON_LEFT])
                clicked = true;
            else if (clicked)
            {
                clicked = false;
                hovering = !hovering;
            }
        }
        else if (mouseButtons[SDL_BUTTON_LEFT])
            if (!util::collide(menuPos, mousePos + renderOffset))
                hovering = false;
    }
    else
        hovering = false;
}

void Interactable::render(Window& window, const Vect<int64_t>& renderOffset)
{
    SDL_Rect render = renderPos;
    render.x -= renderOffset.x;
    render.y -= renderOffset.y;

    std::vector<uint8_t> color = renderColor;
    if (placing) // Alphafied
    {
        color.push_back(alpha);

        if (!placable) // Redified
        {
            color[0] = 255;
            color[1] = 0;
            color[2] = 0;
        }
    }

    window.drawRect(render, color);
}

void Interactable::renderMenu(Window& window, const Vect<int64_t>& renderOffset)
{
    if (hovering)
    {
        setMenuRect();

        SDL_Rect render = menuPos;
        render.x -= renderOffset.x;
        render.y -= renderOffset.y;
        
        window.drawRect(render, { 255, 255, 255 });
    }
}

std::string Interactable::getSave()
{
    if (placing) return RemoveObj; // Removes this because it's placing

    std::string save = objTNames.at(type); // Object name

    save += std::to_string(renderPos.x) + "," + 
            std::to_string(renderPos.y) + "#"; // Divider

    return save;
}

std::string Interactable::readSave(std::string& save)
{
    // Remove name
    save = save.substr(objTNames.at(type).length());

    // Basically inverting what is done in the genSaveData function
    std::vector<std::string> data = util::split(save, ",");

    renderPos.x = std::stoi(data[0]);
    renderPos.y = std::stoi(data[1]);

    return save.substr(save.find("#") + 1); // Removing everything before the divider
}

void Interactable::setMenuRect()
{
    Vect<int> menuSizeInt = menuSize.cast<int>();
    if (renderPos.y + renderPos.w + menuSizeInt.y > static_cast<int>(WIN_HEIGHT))
        menuPos.y = renderPos.y - menuSizeInt.y;
    else
        menuPos.y = renderPos.y + renderPos.w;
    
    menuPos.x = renderPos.x + (renderPos.w / 2) - (menuSizeInt.x / 2);
}