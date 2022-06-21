#include "interactable.h"

#include <iostream>
#include <string>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"
#include "animation.h"
#include "menu.h"

std::unordered_map<ObjType, std::unordered_map<std::string, SDL_Texture*>> Interactable::textures = {};
std::unordered_map<std::string, Menu> Interactable::menus = {};
nlohmann::json Interactable::menuData = nlohmann::json(nlohmann::json::value_t::object);

Interactable::Interactable(Window& window, const nlohmann::json& data, const Vect<uint32_t> tileSize, const ObjType type)
    : currentAnim("idle"), tileSize(tileSize), modColor{255, 255, 255},
      renderPos{ 0, 0, static_cast<int>(tileSize.x * TILE_SIZE), 
                       static_cast<int>(tileSize.y * TILE_SIZE) }, 
      placing(true), placable(true), hovering(false), clicked(false),
      currentMenu("home"), type(type)
{
    loadImgs(window, data);
    setupAnims(window, data);
}

Interactable::Interactable(Window& window, const nlohmann::json& data, const ObjType type)
    : currentAnim("idle"), modColor{255, 255, 255}, 
      placing(false), placable(false), 
      hovering(false), clicked(false),
      currentMenu("home"), type(type)
{
    loadImgs(window, data);
    setupAnims(window, data);
}

Interactable::~Interactable()
{
    for (const auto& tex : textures[type])
        SDL_DestroyTexture(tex.second);
}

void Interactable::loadImgs(Window& window, const nlohmann::json& data)
{
    if (textures.find(type) != textures.end())
        return; 
    
    // Textures stored staticly
    for (const auto& frameData : data["anims"].items())
        textures[type][frameData.key()] = window.scale(window.loadTexture(frameData.value()["path"].get<std::string>().c_str()));
}

void Interactable::setupAnims(Window& window, const nlohmann::json& data)
{
    for (const auto& frameData : data["anims"].items())
        anims.emplace(std::piecewise_construct,
                      std::forward_as_tuple(frameData.key()),
                      std::forward_as_tuple(textures[type][frameData.key()], 
                                            frameData.value()["frames"].get<uint32_t>(), 
                                            frameData.value()["delay"].get<float>()));
}

void Interactable::loadMenuData()
{
    menuData = nlohmann::json::parse(std::ifstream(MENU_DATA_PATH));

    // Loading menus from menu data
    for (const auto& data : menuData["interactables"].items())
        menus.emplace(data.key(), Menu({0, 0}, data.value()));
}

void Interactable::resetTextures(Window& window, const nlohmann::json& allData)
{
    for (const auto& bName : objTNames)
    {
        for (const auto& animData : allData[bName.second]["anims"].items())
        {
            if (textures.find(bName.first) == textures.end()) 
                continue;

            // Rerendering textures onto the scaledup versions
            SDL_Texture* tex = window.loadTexture(animData.value()["path"].get<std::string>().c_str());
            SDL_Rect dst = util::getRect({0, 0}, util::getSize(tex) * SCALE);

            window.setTarget(textures[bName.first][animData.key()]);
            window.render(tex, dst);
            window.resetTarget();

            SDL_DestroyTexture(tex);
        }
    }
}

bool Interactable::canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size)
{
    // If the platform is touching any other object besides itself
    for (std::unique_ptr<Interactable>& obj : objects)
        if (util::collide(renderPos, obj->getRect()) && !obj->isPlacing())
            return false;

    // If the platform is outside of the base boundaries
    if (pos.x < 0 || pos.x + renderPos.w > size.x || 
        pos.y < 0 || pos.y + renderPos.h > size.y)
        return false;

    return true;
}

void Interactable::completePlace(const uint64_t& time)
{
    placing = false; 
}

void Interactable::update(Window& window, const uint64_t& time)
{
    anims.at(currentAnim).update(window);
}

void Interactable::checkMenu(Window& window, const Vect<int64_t>& renderOffset)
{
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
                chooseMenu();
            }
        }
        else if (window.buttonHeld(SDL_BUTTON_LEFT) && hovering)
        {
            if (!util::collide(menus.at(currentMenu).getRect(), mouseMapPos))
                removeMenu();
            else
                window.setButton(SDL_BUTTON_LEFT, false);
        }
        else clicked = false;
    }
    else
        hovering = false;
}

// Override this to modify menu selection
void Interactable::chooseMenu()
{
    currentMenu = "home";
}

void Interactable::render(Window& window, const Vect<int64_t>& renderOffset)
{
    setModColor(window);
    renderCenter(window, renderOffset);
}

void Interactable::renderCenter(Window& window, const Vect<int64_t>& renderOffset)
{
    anims.at(currentAnim).renderCenter(window, getCenter().cast<int64_t>() - renderOffset);
}

void Interactable::renderMenu(Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize)
{
    if (!hovering) return; // End function if the menu isn't open
    
    if (hovering)
        updateMenuPos(window, renderOffset, baseSize);

    menus.at(currentMenu).render(window);

    if (currentMenu == "home")
    {
        // Render some stats here using the Menu object
    }
}

void Interactable::updateAnim(Window& window)
{
    anims.at(currentAnim).update(window);
}

void Interactable::setModColor(Window& window)
{
    std::vector<uint8_t> color = modColor;
    if (placing) // Alphafied
    {
        anims.at(currentAnim).modAlpha(window, ALPHA);

        if (!placable) // Redified
        {
            color[0] = 255;
            color[1] = 50;
            color[2] = 50;
        }
    }
    else anims.at(currentAnim).modAlpha(window, 255);

    anims.at(currentAnim).modColor(window, color);
}

void Interactable::swapAnim(const std::string& name)
{
    anims.at(currentAnim).reset();
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

void Interactable::updateMenuPos(const Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize)
{
    const Vect<int> menuSize = menus.at(currentMenu).getSize().cast<int>();

    SDL_Rect screenPos = renderPos;
    screenPos.x -= renderOffset.x;
    screenPos.y -= renderOffset.y;

    if (screenPos.y + screenPos.h + menuSize.y > static_cast<int>(window.getCamSize().y))
        menus.at(currentMenu).setY(screenPos.y - menuSize.y);
    else
        menus.at(currentMenu).setY(screenPos.y + screenPos.h);
    
    int tileMenuX = (renderPos.x + (renderPos.w / 2) - (menuSize.x / 2)); // Not onscreen position
    if (tileMenuX < 0) 
        menus.at(currentMenu).setX(-renderOffset.x); // Setting to the left of the board
    else if (tileMenuX + menuSize.x > baseSize.xCast<int>())
        menus.at(currentMenu).setX(baseSize.x - menuSize.x - renderOffset.x); // Right side of board
    else
        menus.at(currentMenu).setX(tileMenuX - renderOffset.x); 
}