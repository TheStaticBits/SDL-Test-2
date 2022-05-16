#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"
#include "base.h"
#include "interactable.h"

class Player
{
public:
    Player(Vect<float> pos);
    ~Player();

    void operator=(const Player&) = delete;

    void update(std::unordered_map<SDL_Keycode, bool>& keys, Base& base, float deltaTime);
    void render(Window& window, Vect<int64_t> renderOffset);

    std::string getSave();
    void readSave(std::string save);
    inline bool checkSavePart(std::string save) { return save.substr(0, saveName.length()) == saveName; }

    void updateRect();
    
    inline const Vect<int64_t> getRenderOffset() const { return renderOffset.cast<int64_t>(); }

private:
    inline static const std::string saveName = "Player";

    static constexpr float SPEED = 120.0f; // Pixels per second
    static constexpr float CAM_TIGHTNESS = 3.75f; // How tight the camera is on the player (higher is tighter)
    inline static const Vect<int32_t> CAM_OFFSET = {(int32_t)round(WIN_WIDTH / 2), (int32_t)round(WIN_HEIGHT / 1.5)}; // Camera offset from player

    static constexpr int32_t JUMP_SPEED = -400; 
    static constexpr int32_t GRAVITY = -800; 

    Vect<float> pos;
    Vect<uint32_t> size;
    Vect<float> renderOffset;
    Vect<float> velocity;
    SDL_Rect rect;

    bool jump;
    bool canJump;
    float downCountdown; // Counter for going down a platform
    static constexpr float downCountdownMax = 0.2f; // Time before the hitbox kicks in again after pressing down

    Vect<float> getOffset();

    void collisions(Base& base, std::unordered_map<SDL_Keycode, bool>& keys, float deltaTime);
    int32_t platformCollide(std::vector<std::unique_ptr<Interactable>>& objects, float deltaTime);
};