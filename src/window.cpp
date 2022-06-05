#include "window.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"
#include "utility.h"

Window::Window()
    : quit(false), resizeWin(false), mousePos{0, 0}, 
      window(NULL), renderer(NULL), camera(NULL),
      realWinSize(900, 600), winSize(realWinSize / WIN_SCALE),
      deltaTime(0.0f), lastTime(0), fps(0), scale(WIN_SCALE),
      renderTo(WINDOW)
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Window::~Window()
{
    SDL_DestroyTexture(camera);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::updateCamera(const Vect<uint32_t> baseSize)
{
    // Setting up game box
    Vect<uint32_t> size;
    
    if (baseSize < winSize) size = baseSize;
    else size = winSize;

    camSize = size;
    realCamSize = size * WIN_SCALE;
    camera = createTex(realCamSize.x, realCamSize.y);

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
    SDL_RenderPresent(renderer); // Presenting frame
    
    // Clearing window
    SDL_RenderClear(renderer); 
}

void Window::inputs(const Vect<uint32_t> baseSize)
{
    // Resetting
    mouseButtons.clear();
    resizeWin = false;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true; break;
            
            case SDL_MOUSEBUTTONDOWN:
                mouseButtons[event.button.button] = true; 
                mouseHeldButtons[event.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseHeldButtons[event.button.button] = false; break;
            
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        resize(event.window.data1, event.window.data2, baseSize);
                        resizeWin = true;
                        break;
                    
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        maximize(baseSize);
                }
                break;
            
            default:
                handleKey(event.key.keysym.sym, event.type); 
                break;
        }
    }

    fMouseButtons = mouseButtons;
    fMouseHeldButtons = mouseHeldButtons;

    Vect<int> mouseRetrieve;
    SDL_GetMouseState(&mouseRetrieve.x, &mouseRetrieve.y);
    // Adjusting based on the scale of the screen
    mousePos = mouseRetrieve.cast<int64_t>() / WIN_SCALE;
    camMousePos = mousePos - (camOffset / WIN_SCALE).cast<int64_t>();
}

void Window::calcDeltaTime()
{
    uint32_t currentTime = SDL_GetTicks();
    
    if ((lastTime % 1000) > (currentTime % 1000)) // Every second
    {
        float total = 0.0f;
        for (auto& it : deltaTimes)
            total += it;
        fps = 1.0f / (total / deltaTimes.size());
        deltaTimes.clear();
    }
    
    deltaTime = (currentTime - lastTime) / 1000.0f;
    deltaTimes.push_back(deltaTime);
    lastTime = currentTime;
}

void Window::modColor(SDL_Texture* texture, std::vector<uint8_t> color)
{
    if (SDL_SetTextureColorMod(texture, color[0], color[1], color[2]) != 0)
        std::cout << "[Error] Unable to set texture mod color: " << SDL_GetError() << std::endl;
}

void Window::modAlpha(SDL_Texture* texture, uint8_t alpha)
{
    if (SDL_SetTextureAlphaMod(texture, alpha) != 0)
        std::cout << "[Error] Unable to set texture alpha: " << SDL_GetError() << std::endl;
}

void Window::render(SDL_Texture* texture, SDL_Rect& dst)
{
    SDL_Rect newRect = scaleUp(dst);
    if (SDL_RenderCopy(renderer, texture, NULL, &newRect) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::render(SDL_Texture* texture, SDL_Rect& dst, SDL_Rect& src)
{
    SDL_Rect newRect = scaleUp(dst);
    if (SDL_RenderCopy(renderer, texture, &src, &newRect) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::render(SDL_Texture* texture, SDL_Rect& dst, const double angle)
{
    SDL_Rect newRect = scaleUp(dst);
    if (SDL_RenderCopyEx(renderer, texture, NULL, &newRect, angle, NULL, SDL_FLIP_NONE) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::renderWithoutScale(SDL_Texture* texture, SDL_Rect& dst)
{
    if (SDL_RenderCopy(renderer, texture, NULL, &dst) != 0)
        std::cout << "[Error] Rendering failed: " << SDL_GetError() << std::endl;
}

void Window::drawRect(SDL_Rect& rect, std::vector<uint8_t> color)
{
    SDL_Rect newRect = scaleUp(rect);

    if (color.size() == 3)
        color.push_back(255);

    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);

    if (SDL_RenderFillRect(renderer, &newRect) != 0)
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
    if (renderTo == WINDOW)
        SDL_SetRenderTarget(renderer, NULL);
    else if (renderTo == CAMERA)
        SDL_SetRenderTarget(renderer, camera);
}

void Window::resize(int32_t width, int32_t height, const Vect<uint32_t> baseSize)
{
    realWinSize = Vect<uint32_t>(width, height);
    winSize = realWinSize / WIN_SCALE;
    SDL_SetWindowSize(window, realWinSize.x, realWinSize.y);
    updateCamera(baseSize);
}

void Window::maximize(const Vect<uint32_t> baseSize)
{
    SDL_MaximizeWindow(window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    resize(DM.w, DM.h, baseSize);
}

void Window::startRenderGame()
{
    renderTo = CAMERA;
    setTarget(camera);
}

void Window::startRenderUI()
{
    renderTo = WINDOW;
    resetTarget();

    SDL_Rect dst;

    // If the camera size is less than window size
    // Centering cam view
    if (realCamSize < realWinSize)
        camOffset = (realWinSize / 2) - (realCamSize / 2);
    else
        camOffset = { 0, 0 };

    dst = { camOffset.xCast<int>(), camOffset.yCast<int>(), realCamSize.xCast<int>(), realCamSize.yCast<int>() };
        
    renderWithoutScale(camera, dst);
}

void Window::handleKey(SDL_Keycode& key, Uint32& type)
{
    if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end())
        keys[key] = (type == SDL_KEYDOWN);
}

SDL_Rect Window::scaleUp(SDL_Rect rect)
{
    rect.x *= scale; rect.y *= scale;
    rect.w *= scale; rect.h *= scale;
    return rect;
}