#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"

inline const int R_WIN_WIDTH = 900; // Real Window Width
inline const int R_WIN_HEIGHT = 600; // Real Window Height

inline const int WIN_WIDTH = 300; // Mini Window Width (Scaled up to real window)
inline const int WIN_HEIGHT = 200; // Mini Window Height (Scaled up to real window)

class Window
{
public:
    Window();
    ~Window();

    void operator=(const Window&) = delete;

    SDL_Texture* loadTexture(const char* path);

    void update();

    // Render a texture to the window
    void render(SDL_Texture* texture, SDL_Rect& pos);
    void render(SDL_Texture* texture, SDL_Rect& src, SDL_Rect& pos);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* mini;

    static constexpr char* title = (char*)"Game again";
};