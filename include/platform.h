#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

template <typename T>
struct Vect;
class Window;
#include "interactable.h"

class Platform : public Interactable
{
public:
    Platform(Window& window, const nlohmann::json& data, const uint32_t tileWidth);
    Platform(Window& window, const nlohmann::json& data, std::string save); // Loading from save
    ~Platform();

    void initParts();

    void operator=(const Platform&) = delete;

    bool canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size) override;

    // void update(const uint64_t& seconds) override;
    void render(Window& window, const Vect<int64_t>& renderOffset) override;

    std::string getSave() override;
    std::string readSave(std::string& save) override;

private:
    static const ObjType type = Platform_T;
    const nlohmann::json& data;

    SDL_Rect head; // Either end of the platform
    SDL_Rect body; // Any central parts of the platform
};