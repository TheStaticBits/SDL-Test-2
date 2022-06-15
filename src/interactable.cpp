#include "interactable.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"
#include "animation.h"

std::unordered_map<ObjType, std::unordered_map<std::string, SDL_Texture*>> Interactable::textures = {};

Interactable::Interactable(Window& window, const nlohmann::json& data, const Vect<uint32_t> tileSize, const ObjType type)
    : currentAnim("idle"), tileSize(tileSize), modColor{255, 255, 255},
      renderPos{ 0, 0, static_cast<int>(tileSize.x * TILE_SIZE), 
                       static_cast<int>(tileSize.y * TILE_SIZE) }, 
      placing(true), placable(true), hovering(false), clicked(false),
      menuSize{90, 70}, type(type)
{
    loadImgs(window, data);
    setupAnims(window, data);
}

Interactable::Interactable(Window& window, const nlohmann::json& data, const ObjType type)
    : currentAnim("idle"), modColor{255, 255, 255}, 
      placing(false), placable(false), 
      hovering(false), clicked(false),
      menuSize{90, 70}, type(type)
{
    loadImgs(window, data);
    setupAnims(window, data);
}

Interactable::~Interactable()
{
    // SDL_DestroyTexture(image);
}

void Interactable::loadImgs(Window& window, const nlohmann::json& data)
{
    if (textures.find(type) != textures.end())
        return; 
    
    // Textures stored staticly
    for (const auto& frameData : data["anims"].items())
        textures[type][frameData.key()] = window.loadTexture(frameData.value()["path"].get<std::string>().c_str());
}

void Interactable::setupAnims(Window& window, const nlohmann::json& data)
{
    for (const auto& frameData : data["anims"].items())
        anims[frameData.key()] = std::make_unique<Animation>(textures[type][frameData.key()], 
                                frameData.value()["frames"].get<uint32_t>(), 
                                frameData.value()["delay"].get<float>());
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

void Interactable::update(Window& window, const uint64_t& time)
{
    anims[currentAnim]->update(window);
}

void Interactable::checkMenu(Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize)
{
    setMenuRect(window, renderOffset, baseSize);

    if (!placing)
    {
        const Vect<int64_t> mouseMapPos = window.getCamMousePos() + renderOffset;
        if (util::collide(renderPos, mouseMapPos))
        {
            if (window.buttonHeld(SDL_BUTTON_LEFT))
                clicked = true;
            else if (clicked)
            {
                clicked = false;
                hovering = !hovering;
            }
        }
        else if (window.buttonHeld(SDL_BUTTON_LEFT) && hovering)
        {
            if (!util::collide(menuPos, mouseMapPos))
                removeMenu();
            else
                window.setButton(SDL_BUTTON_LEFT, false);
        }
    }
    else
        hovering = false;
}

void Interactable::render(Window& window, const Vect<int64_t>& renderOffset)
{
    setModColor(window);

    anims[currentAnim]->renderCenter(window, getCenter().cast<int64_t>() - renderOffset);
}

void Interactable::renderMenu(Window& window, const Vect<int64_t>& renderOffset)
{
    if (hovering)
    {
        SDL_Rect render = menuPos;
        render.x -= renderOffset.x;
        render.y -= renderOffset.y;

        window.drawRect(render, { 255, 255, 255 });
    }
}

void Interactable::updateAnim(Window& window)
{
    anims[currentAnim]->update(window);
}

void Interactable::setModColor(Window& window)
{
    std::vector<uint8_t> color = modColor;
    if (placing) // Alphafied
    {
        anims[currentAnim]->modAlpha(window, ALPHA);

        if (!placable) // Redified
        {
            color[0] = 255;
            color[1] = 0;
            color[2] = 0;
        }
    }

    anims[currentAnim]->modColor(window, color);
}

void Interactable::swapAnim(const std::string& name)
{
    anims[currentAnim]->reset();
    currentAnim = name;
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

void Interactable::setMenuRect(const Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize)
{
    const Vect<int> menuSizeInt = menuSize.cast<int>();
    menuPos.w = menuSizeInt.x;
    menuPos.h = menuSizeInt.y;

    SDL_Rect renderScreenPos = renderPos;
    renderScreenPos.x -= renderOffset.x;
    renderScreenPos.y -= renderOffset.y;

    if (renderScreenPos.y + renderScreenPos.h + menuPos.h > static_cast<int>(window.getCamSize().y))
        menuPos.y = renderPos.y - menuPos.h;
    else
        menuPos.y = renderPos.y + renderPos.h;
    
    menuPos.x = renderPos.x + (renderPos.w / 2) - (menuPos.w / 2);
    if (menuPos.x < 0)
        menuPos.x = 0;
    else if (menuPos.x + menuPos.w > baseSize.xCast<int>())
        menuPos.x = baseSize.x - menuPos.w;
}