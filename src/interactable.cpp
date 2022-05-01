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

bool Interactable::genCanPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size)
{
    std::cout << objects.size() << std::endl;
    // If the platform is touching any other object besides itself
    for (std::unique_ptr<Interactable>& obj : objects)
    {
        if (util::collide(renderPos, obj->getRect()) && !obj->isPlacing())
            return false;
    }

    // If the platform is outside of the base boundaries
    if (pos.x < 0 || 
        pos.x + renderPos.w > size.x || 
        pos.y < 0 || 
        pos.y + renderPos.h > size.y)
        return false;

    return true;
}


void Interactable::render(Window& window, Vect<int>& renderOffset)
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