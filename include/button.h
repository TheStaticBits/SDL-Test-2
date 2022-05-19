#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

enum bTextures { ButtonBlue };
inline std::unordered_map<bTextures, std::string> bFolderNames = {
    { ButtonBlue, "blue" }
};
inline std::vector<std::string> bImgStates = { "idle", "hovering", "pressed" };

class Button
{
public:
    Button(Window& window, bTextures texType, 
           Vect<int64_t> pos, std::string text = "",
           const uint32_t fontSize = 0,
           SDL_Color textColor = {255, 255, 255, 255});
    ~Button();

    void operator=(const Button&) = delete;

    void setupText(std::string text, const uint32_t fontSize, SDL_Color color);

    void update(const Vect<int64_t>& mousePos,    
                const std::unordered_map<uint8_t, bool>& mouseButtons);
    void render(Window& window);
    
    inline void setPos(Vect<int64_t> pos) { pos = pos; }

    inline const bool isActivated() const { return activated; }

private:
    static std::unordered_map<bTextures, std::unordered_map<std::string, SDL_Texture*>> textures;
    
    bTextures texType;
    SDL_Texture* textImg;
    
    Vect<uint32_t> size; 
    Vect<int64_t> pos;

    SDL_Rect rect;
    void updateRect();

    bool hovering;
    bool pressed;
    bool activated;
};