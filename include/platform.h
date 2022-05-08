#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "base.h"
#include "interactable.h"
#include "utility.h"

class Platform : public Interactable
{
public:
    Platform(const int tileWidth);
    Platform(std::string save); // Loading from save
    ~Platform();

    void operator=(const Platform&) = delete;

    bool canPlace(const Vect<int>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<int>& size, nlohmann::json bData) override;

    void update(std::time_t seconds) override;
    void render(Window& window, Vect<int> renderOffset) override;

    std::string getSave() override;
    void readSave(std::string save);

private:

};