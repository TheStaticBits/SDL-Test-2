#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Window;
class Base;
#include "interactable.h"

class Player
{
public:
    Player(const Window& window);
    ~Player();

    void operator=(const Player&) = delete;

    void update(Window& window, Base& base);
    void render(Window& window, const Vect<int64_t> renderOffset);

    std::string getSave();
    void readSave(std::string save, const Window& window);
    inline bool checkSavePart(std::string save) { return save.substr(0, saveName.length()) == saveName; }

    void updateRect();

    void resize(Base& base, const Window& window);
    
    inline const Vect<int64_t> getRenderOffset() const { return renderOffset.cast<int64_t>(); }

private:
    inline static const std::string saveName = "Player";

    static constexpr float SPEED = 320.0f; // Pixels per second
    static constexpr float CAM_TIGHTNESS = 3.75f; // How tight the camera is on the player (higher is tighter)

    static constexpr int32_t JUMP_SPEED = -1250; 
    static constexpr int32_t GRAVITY = -2400; 

    Vect<float> pos;
    Vect<uint32_t> size;
    Vect<float> renderOffset;
    Vect<float> velocity;
    SDL_Rect rect;

    bool jump;
    bool canJump;
    float downCountdown; // Counter for going down a platform
    static constexpr float downCountdownMax = 0.2f; // Time before the hitbox kicks in again after pressing down

    Vect<float> getOffset(const Window& window);
    void lockOffset(Base& base, const Window& window);

    void collisions(Window& window, Base& base, const float deltaTime);
    int32_t platformCollide(std::vector<std::unique_ptr<Interactable>>& objects, float deltaTime);
};