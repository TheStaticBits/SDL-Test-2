#include "button.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

Button::Button(Window& window, bTextures textureColor, 
               Vect<int64_t> pos, std::string text,
               const uint32_t fontSize, SDL_Color textColor)
    : pos(pos),
      hovering(false), clicking(false), activated(false)
{
    if (text != "")
    {
        TTF_Font* font = window.font(fontSize);
        textImg = window.getTextImg(font, text, textColor);
        TTF_CloseFont(font);
    }

    // Button loading code, check if the color is loaded, if not, load it
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
            clicking = true;
        else if (clicking)
        {
            clicking = false;
            activated = true;
        }
    }
    else
        clicking = false;
}

void Button::render(Window& window)
{

}

void Button::updateRect()
{
    Vect<int> sizeInt = size.cast<int>();
    Vect<int> posInt = pos.cast<int>();
    rect = { posInt.x, posInt.y, sizeInt.x, sizeInt.y };
}