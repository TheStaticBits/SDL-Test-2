#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

template <typename T>
struct Vect;
class Window;
#include "interactable.h"

class Platform : public Interactable
{
public:
    Platform(const uint32_t tileWidth);
    Platform(std::string save); // Loading from save
    ~Platform();

    void operator=(const Platform&) = delete;

    bool canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size) override;

    // void update(const uint64_t& seconds) override;
    void render(Window& window, const Vect<int64_t>& renderOffset) override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;

private:
    static const ObjType type = Platform_T;
};