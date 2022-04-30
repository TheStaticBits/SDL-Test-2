#include "base.h"

#include <iostream>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "interactable.h"

Base::Base()
    : size{R_WIN_WIDTH, R_WIN_HEIGHT}, placing(false)
{

}

Base::~Base()
{

}

void Base::update(std::unordered_map<SDL_Keycode, bool>& keys, 
                  std::unordered_map<Uint8, bool>& mouseButtons, 
                  Vect<int>& mousePos,
                  Vect<int>& renderOffset)
{
    // Temporary
    if (keys[SDLK_SPACE])
    {
        placing = true;
        objects.push_back(Interactable({40, 40}, {2, 2}, {255, 255, 255}));
    }

    for (Interactable& obj : objects)
        obj.update();

    if (placing)
    {
        // Getting the tile the mouse is hovering over
        Vect<int> placeTile;
        placeTile.x = (int)floor((mousePos.x + renderOffset.x) / TILE_SIZE);
        placeTile.y = (int)floor((mousePos.y + renderOffset.y) / TILE_SIZE);

        // Changing floor to ceil for negatives
        if (mousePos.x + renderOffset.x < 0)
            placeTile.x -= 1;
        if (mousePos.y + renderOffset.y < 0)
            placeTile.y -= 1;

        for (Interactable& obj : objects)
        {
            if (obj.isPlacing())
            {
                // Centering
                placeTile.x -= (int)floor(obj.getTileSize().x / 2);
                placeTile.y -= (int)floor(obj.getTileSize().y / 2);

                // placeTile.print();

                Vect<int> screenPos;
                screenPos.x = placeTile.x * TILE_SIZE;
                screenPos.y = placeTile.y * TILE_SIZE;

                obj.setPos(screenPos);

                if (obj.canPlace(objects))
                {
                    obj.setPlacable(true);

                    if (mouseButtons[SDL_BUTTON_LEFT])
                    {
                        placing = false;
                        obj.completePlace();
                    }
                }
                else
                    obj.setPlacable(false);
            }
        }
    }
    //else std::cout << "Not placing" << std::endl;

    for (Interactable& obj : objects)
        obj.update();
}

void Base::render(Window& window, Vect<int> renderOffset)
{
    for (Interactable& obj : objects)
        obj.render(window, renderOffset);
}