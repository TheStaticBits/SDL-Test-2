#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"
#include "utility.h"

inline const uint32_t R_WIN_WIDTH = 900; // Real Window Width
inline const uint32_t R_WIN_HEIGHT = 600; // Real Window Height

inline const uint32_t WIN_WIDTH = 300; // Mini Window Width (Scaled up to real window)
inline const uint32_t WIN_HEIGHT = 200; // Mini Window Height (Scaled up to real window)
inline const uint32_t WIN_SCALE = 3;

class Window
{
public:
    Window();
    ~Window();

    void operator=(const Window&) = delete;

    SDL_Texture* loadTexture(const char* path);
    TTF_Font* font(const uint32_t size);
    SDL_Texture* getTextImg(TTF_Font* font, std::string text, SDL_Color color);

    void update();

    // Render a texture to the window
    void render(SDL_Texture* texture, SDL_Rect& pos);
    void render(SDL_Texture* texture, SDL_Rect& src, SDL_Rect& pos);
    
    void drawRect(SDL_Rect& rect, std::vector<uint8_t> color);

private:
    static constexpr char* TITLE = (char*)"Game again";
    static constexpr char* FONT_PATH = (char*)"res/font/font.ttf";
    static constexpr bool VSYNC = true;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* mini; // Scaled up to full size when before presenting
    std::unordered_map<uint32_t, TTF_Font*> fonts; // For different sizes
};