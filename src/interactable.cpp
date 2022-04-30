#include "interactable.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"

Interactable::Interactable(Vect<int> size, Vect<int> tileSize, std::vector<Uint8> color)
    : renderPos{0, 0, size.x, size.y}, tileSize(tileSize), pos(0, 0), 
      renderColor(color), placing(true), placable(true), hovering(false)
{

}

Interactable::~Interactable()
{
    // SDL_DestroyTexture(image);
}

void Interactable::update()
{
}

void Interactable::render(Window& window, Vect<int>& renderOffset)
{
    SDL_Rect render = renderPos;
    render.x -= renderOffset.x;
    render.y -= renderOffset.y;

    std::vector<Uint8> color = renderColor;
    if (placing) // Alphafied
    {
        color[3] = 128;

        if (!placable) // Redified
        {
            color[0] = 255;
            color[1] = 0;
            color[2] = 0;
        }
    }

    window.drawRect(render, color);
}