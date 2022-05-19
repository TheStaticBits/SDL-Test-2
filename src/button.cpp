#include "button.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

Button::Button(Window& window, bTextures texType, 
               Vect<int64_t> pos, std::string text,
               const uint32_t fontSize, SDL_Color textColor)
    : texType(texType), pos(pos),
      hovering(false), pressed(false), activated(false)
{
    if (text != "")
    {
        TTF_Font* font = window.font(fontSize);
        textImg = window.getTextImg(font, text, textColor);
        TTF_CloseFont(font);
    }

    if (textures.find(texType) == textures.end())
    {
        textures[texType] = {}; // Test to see if this is needed!
        
        for (std::string& state : bImgStates)
        {
            textures[texType][state] = window.loadTexture((std::string("res/buttons/") + 
                                                           bFolderNames.at(texType)    +
                                                           std::string("/") + state    + 
                                                           std::string(".png")).c_str());
        }
    }

    size = util::getSize(textures[texType]["idle"]);
}

Button::~Button()
{
    if (textImg != NULL) SDL_DestroyTexture(textImg);
    
    // Destroy images 
}

void Button::update(const Vect<int64_t>& mousePos,    
                    const std::unordered_map<uint8_t, bool>& mouseButtons)
{
    updateRect();

    activated = false;
    hovering = util::collide(rect, mousePos);
    
    if (hovering)
    {
        if (mouseButtons.at(SDL_BUTTON_LEFT))
            pressed = true;
        else if (pressed)
        {
            pressed = false;
            activated = true;
        }
    }
    else
        pressed = false;
}

void Button::render(Window& window)
{
    if (hovering)     window.render(textures[texType]["hovering"], rect);
    else if (pressed) window.render(textures[texType]["pressed"],  rect);
    else              window.render(textures[texType]["idle"],     rect);
}

void Button::updateRect()
{
    Vect<int> sizeInt = size.cast<int>();
    Vect<int> posInt = pos.cast<int>();
    rect = { posInt.x, posInt.y, sizeInt.x, sizeInt.y };
}