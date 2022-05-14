#include "window.h"

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "utility.h"

Window::Window()
    : window(NULL), renderer(NULL)
{
    // Setting up window
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, R_WIN_WIDTH, R_WIN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
        std::cout << "[Error] Window creation failed: " << SDL_GetError() << std::endl;
    
    // Setting up renderer
    if (VSYNC)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    else
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    // Setting up mini window
    mini = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mini, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, mini); 
}

Window::~Window()
{
    std::cout << "[Info] Window destroyed" << std::endl;

    SDL_DestroyTexture(mini);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Texture* Window::loadTexture(const char* path)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL)
        std::cout << "[Error] Texture loading failed: " << SDL_GetError() << std::endl;

    return texture;
}

void Window::update()
{
    SDL_SetRenderTarget(renderer, NULL); // Setting to default renderer

    // Updating screen with the frame
    SDL_Rect pos{0, 0, R_WIN_WIDTH, R_WIN_HEIGHT};
    render(mini, pos); // Rendering mini window to renderer
    SDL_RenderPresent(renderer); // Presenting frame

    // Clearing windows
    // SDL_RenderClear(renderer); // Cause of flickering...
    SDL_SetRenderTarget(renderer, mini); // Setting to mini window
    SDL_RenderClear(renderer); // Clearing the mini window

    // Any rendering from here will render to the mini window
}

void Window::render(SDL_Texture* texture, SDL_Rect& pos)
{
    if (SDL_RenderCopy(renderer, texture, NULL, &pos) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::render(SDL_Texture* texture, SDL_Rect& src, SDL_Rect& pos)
{
    if (SDL_RenderCopy(renderer, texture, &src, &pos) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::drawRect(SDL_Rect& rect, std::vector<uint8_t> color)
{
    if (color.size() == 4)
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
    else
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);

    if (SDL_RenderFillRect(renderer, &rect) != 0)
        std::cout << "[Error] Rendering rect failed: " << SDL_GetError() << std::endl;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}