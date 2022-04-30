#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"

// Objects in the home base that can be interacted with
class Interactable
{
public:
    Interactable(Vect<int> size, Vect<int> tileSize, std::vector<Uint8> color);
    ~Interactable();

    void operator=(const Interactable&) = delete;

    // virtual bool canPlace(const Vect<int>& pos, std::vector<Interactable> otherObjects) = 0;
    bool canPlace(const std::vector<Interactable>& otherObjects) const { return true; } // For now, so this class isn't Abstract

    void update();

    void render(Window& window, Vect<int> renderOffset);

    inline bool isPlacing() const { return placing; }
    inline void setPlacable(bool canPlace) { placable = canPlace; }
    inline Vect<int> getTileSize() const { return tileSize; }
    inline void setPos(Vect<int> pos) { renderPos.x = pos.x; renderPos.y = pos.y; }
    inline void completePlace() { placing = false; }

private:
    static constexpr int hoveringAlpha = 128;

    // SDL_Texture* image; // Add image later
    SDL_Rect renderPos; // Top left corner
    Vect<int> tileSize; // Amount of tiles it takes up
    Vect<int> pos;
    std::vector<Uint8> renderColor;

    bool placing;
    bool placable;
    bool hovering;
};