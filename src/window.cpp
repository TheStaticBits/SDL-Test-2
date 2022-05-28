#include "window.h"

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"
#include "utility.h"

Window::Window()
    : window(NULL), renderer(NULL), mini(NULL),
      realWinSize(900, 600), winSize(realWinSize / WIN_SCALE)
{
    // Setting up window
    window = SDL_CreateWindow(TITLE, 
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              realWinSize.x, realWinSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        std::cout << "[Error] Window creation failed: " << SDL_GetError() << std::endl;
    
    SDL_SetWindowMinimumSize(window, realWinSize.x, realWinSize.y);

    uint32_t flags;
    
    // Setting up renderer
    if (VSYNC)
        flags = ( SDL_RENDERER_ACCELERATED | 
                  SDL_RENDERER_PRESENTVSYNC | 
                  SDL_RENDERER_TARGETTEXTURE );
    else
        flags = ( SDL_RENDERER_ACCELERATED | 
                  SDL_RENDERER_TARGETTEXTURE );


    renderer = SDL_CreateRenderer(window, -1, flags);

    createMini();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, mini); 
}

Window::~Window()
{
    SDL_DestroyTexture(mini);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Texture* Window::loadTexture(const char* path)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL)
        std::cout << "[Error] Texture loading failed: " << IMG_GetError() << std::endl;

    return texture;
}

TTF_Font* Window::font(const uint32_t size)
{
    if (fonts.find(size) != fonts.end())
        return fonts.at(size);
    
    TTF_Font* font = TTF_OpenFont(FONT_PATH, size);

    if (!font)
        std::cout << "[Error] Font loading failed: " << TTF_GetError() << std::endl;
    
    fonts[size] = font;

    return font;
}

SDL_Texture* Window::createTex(const uint32_t width, const uint32_t height)
{
    SDL_Texture* tex = NULL;
    tex = SDL_CreateTexture(renderer, 
                            SDL_PIXELFORMAT_RGBA8888, 
                            SDL_TEXTUREACCESS_TARGET, 
                            width, height);
    if (tex == NULL)
        std::cout << "[Error] Texture creation failed: " << SDL_GetError() << std::endl;
    
    return tex;
}

SDL_Texture* Window::getTextImg(TTF_Font* font, std::string text, SDL_Color color)
{
    SDL_Texture* textTexture = NULL;
    SDL_Surface* textSurf = TTF_RenderText_Blended(font, text.c_str(), color);

    if (!textSurf)
        std::cout << "[Error] Unable to render font: " << TTF_GetError() << std::endl;
    else
    {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurf);
        SDL_FreeSurface(textSurf);
    }
    
    if (!textTexture)
        std::cout << "[Error] Unable to create texture from surf: " << SDL_GetError() << std::endl;
    
    return textTexture;
}

void Window::update()
{
    SDL_SetRenderTarget(renderer, NULL); // Setting to default renderer

    Vect<int> rWinSizeInt = realWinSize.cast<int>();
    // Updating screen with the frame
    SDL_Rect pos{0, 0, rWinSizeInt.x, rWinSizeInt.y};
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
    if (color.size() == 3)
        color.push_back(255);

    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);

    if (SDL_RenderFillRect(renderer, &rect) != 0)
        std::cout << "[Error] Rendering rect failed: " << SDL_GetError() << std::endl;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Window::setTarget(SDL_Texture* texture)
{
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);   
    SDL_SetRenderTarget(renderer, texture);
}

void Window::resetTarget()
{
    SDL_SetRenderTarget(renderer, mini);
}

void Window::resize(int32_t width, int32_t height)
{
    realWinSize = Vect<uint32_t>(width, height);
    winSize = realWinSize / WIN_SCALE;
    createMini();
    SDL_SetWindowSize(window, realWinSize.x, realWinSize.y);
}

void Window::maximize()
{
    SDL_MaximizeWindow(window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    resize(DM.w, DM.h);
}

void Window::createMini()
{
    if (mini != NULL) SDL_DestroyTexture(mini);

    mini = createTex(winSize.x, winSize.y);
    SDL_SetTextureBlendMode(mini, SDL_BLENDMODE_BLEND);
}