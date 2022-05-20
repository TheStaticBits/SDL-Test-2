#include "button.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

std::unordered_map<bTextures, std::unordered_map<std::string, SDL_Texture*>> Button::textures = {};

Button::Button(Window& window, bTextures texType, 
               Vect<int64_t> pos, std::string text,
               const uint32_t fontSize, SDL_Color textColor)
    : texType(texType), textImg(NULL), pos(pos),
      hovering(false), pressed(false), activated(false)
{
    if (text != "")
    {
        TTF_Font* font = window.font(fontSize);
        textImg = window.getTextImg(font, text, textColor);
        TTF_CloseFont(font);

        textSize = util::getSize(textImg);
    }

    if (textures.find(texType) == textures.end())
    {
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
    
    for (std::string& state : bImgStates)
        SDL_DestroyTexture(textures[texType][state]);
}

void Button::update(const Vect<int64_t>& mousePos,    
                    std::unordered_map<uint8_t, bool>& mouseHeldButtons)
{
    updateRect();

    activated = false;
    hovering = util::collide(rect, mousePos);
    
    if (hovering)
    {
        if (mouseHeldButtons[SDL_BUTTON_LEFT])
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

void Button::render(Window& window, Vect<int64_t> textOffset)
{
    // Draw button
    if (hovering && !pressed) window.render(textures[texType]["hovering"], rect);
    else if (pressed)         window.render(textures[texType]["pressed"],  rect);
    else                      window.render(textures[texType]["idle"],     rect);
    
    // Draw text
    if (textImg != NULL)
    {   
        Vect<int> sizeInt = size.cast<int>();
        Vect<int> textSizeInt = textSize.cast<int>();
        Vect<int> textOffsetInt = textOffset.cast<int>();
        Vect<int> posInt = pos.cast<int>();
        
        SDL_Rect textRect = { posInt.x + (sizeInt.x / 2) - (textSizeInt.x / 2) + textOffsetInt.x, 
                              posInt.y + (sizeInt.y / 2) - (textSizeInt.y / 2) + textOffsetInt.y, 
                              textSizeInt.x, textSizeInt.y };

        window.render(textImg, textRect);
    }
}

void Button::updateRect()
{
    Vect<int> sizeInt = size.cast<int>();
    Vect<int> posInt = pos.cast<int>();
    rect = { posInt.x, posInt.y, sizeInt.x, sizeInt.y };
}