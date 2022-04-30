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
        objects.push_back(Interactable({20, 20}, {1, 1}, {255, 255, 255}));
    }

    for (Interactable& obj : objects)
        obj.update();

    if (placing)
    {
        // Getting the tile the mouse is hovering over
        Vect<int> placeTile;
        placeTile.x = (int)floor((mousePos.x + renderOffset.x) / TILE_SIZE);
        placeTile.y = (int)floor((mousePos.y + renderOffset.y) / TILE_SIZE);

        for (Interactable& obj : objects)
        {
            if (obj.isPlacing())
            {
                // Centering
                placeTile.x -= (int)floor(obj.getTileSize().x / 2);
                placeTile.y -= (int)floor(obj.getTileSize().y / 2);

                Vect<int> screenPos;
                screenPos.x = placeTile.x * TILE_SIZE - renderOffset.x;
                screenPos.y = placeTile.y * TILE_SIZE - renderOffset.y;

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
}

void Base::render(Window& window, Vect<int> renderOffset)
{
    for (Interactable& obj : objects)
        obj.render(window, renderOffset);
}