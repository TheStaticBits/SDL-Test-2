#include "building.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "interactable.h"

Building::Building()
    : buildingTimer(0)
{

}

Building::~Building()
{
    
}

bool Building::canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData)
{
    // place on platform 
}

void Building::update()
{
    // timer updating and whatnot
}

void Building::render(Window& window, Vect<int> renderOffset)
{
    // more code here?? perhaps
    Interactable::genRender();
}