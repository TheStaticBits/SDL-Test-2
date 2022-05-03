#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "base.h"
#include "interactable.h"

// Contains more specific code, for each building
// Buildings will inherit from this class
class Building : public Interactable
{
public:
    Building(Vect<int> tileSize, std::vector<Uint8>& color);
    ~Building();

    void operator=(const Building&) = delete;

    bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData) override;

    void update() override;
    void render(Window& window, Vect<int> renderOffset) override;

protected:
    int buildingTimer;
};