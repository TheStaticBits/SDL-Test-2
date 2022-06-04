#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"

inline const uint32_t WIN_SCALE = 3;

class Window
{
public:
    Window();
    ~Window();

    void operator=(const Window&) = delete;

    SDL_Texture* loadTexture(const char* path);
    TTF_Font* font(const uint32_t size);
    SDL_Texture* createTex(const uint32_t width, const uint32_t height);
    SDL_Texture* getTextImg(TTF_Font* font, std::string text, SDL_Color color);

    void update();
    void inputs();
    void calcDeltaTime();

    // Mod texture colors
    void modColor(SDL_Texture* tex, std::vector<uint8_t> color);

    // Render a texture to the window
    void render(SDL_Texture* texture, SDL_Rect& dst);
    void render(SDL_Texture* texture, SDL_Rect& src, SDL_Rect& dst);
    void render(SDL_Texture* texture, SDL_Rect& dst, const double angle);
    
    void drawRect(SDL_Rect& rect, std::vector<uint8_t> color);

    void setTarget(SDL_Texture* texture);
    void resetTarget();

    void resize(int32_t width, int32_t height);
    void maximize();

    // Getters
    inline Vect<uint32_t> getSize() const { return winSize;          }
    inline const bool isClosed() const    { return quit;             }
    inline const bool getResize() const   { return resizeWin;        }

    inline const bool pKey(const SDL_Keycode key)      { return keys[key];                }
    inline const bool pKeyOnce(const SDL_Keycode key)  { return oneTimeKeys[key];         }
    inline const bool button(const uint8_t button)     { return fMouseButtons[button];     }
    inline const bool buttonHeld(const uint8_t button) { return fMouseHeldButtons[button]; }

    inline const Vect<int64_t> getMousePos() const { return mousePos; }
    inline const float getDeltaTime() const        { return deltaTime; }
    inline const float getFPS() const              { return fps; }

    // Setters
    inline void setButton(const uint8_t button, const bool state)     
    {
        fMouseButtons[button] = state;
        fMouseHeldButtons[button] = state; 
    }
    inline void setScale(const uint32_t newScale) { scale = newScale; }

private:
    void handleKey(SDL_Keycode& key, Uint32& type);

    SDL_Rect scaleUp(SDL_Rect rect);

    static constexpr char* TITLE = (char*)"Game again";
    static constexpr char* FONT_PATH = (char*)"res/font/font.ttf";
    static constexpr bool VSYNC = true;
    inline static const std::vector<SDL_Keycode> allowedKeys = {
        SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_RETURN, SDLK_SPACE,
        SDLK_b, SDLK_p // Temporary, for placing buildings/platforms
    };

    bool quit;
    bool resizeWin;

    std::unordered_map<SDL_Keycode, bool> keys;
    std::unordered_map<SDL_Keycode, bool> oneTimeKeys;
    std::unordered_map<uint8_t, bool> mouseButtons;
    std::unordered_map<uint8_t, bool> mouseHeldButtons;
    Vect<int64_t> mousePos;
    
    // Frame maps, can be modified so you can't click through things
    std::unordered_map<uint8_t, bool> fMouseButtons;
    std::unordered_map<uint8_t, bool> fMouseHeldButtons;

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unordered_map<uint32_t, TTF_Font*> fonts; // For different sizes

    Vect<uint32_t> realWinSize;
    Vect<uint32_t> winSize;

    float deltaTime;
    uint32_t lastTime;
    std::vector<float> deltaTimes;
    float fps;
    uint32_t scale;
};