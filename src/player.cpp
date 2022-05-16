#include "player.h"

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

Player::Player(Vect<float> pos)
    : pos(pos), size{20, 20}, renderOffset(getOffset()), velocity{0, 0}, jump(false), canJump(false)
{

}

Player::~Player()
{
    // Destroy image/animations (currently none)
}

void Player::update(std::unordered_map<SDL_Keycode, bool>& keys, Base& base, float deltaTime)
{
    velocity.x = (keys[SDLK_d] - keys[SDLK_a]) * SPEED * deltaTime;

    if (keys[SDLK_w] && !jump && canJump)
    {
        velocity.y += JUMP_SPEED;
        jump = true;
        canJump = false;
    }

    velocity.y -= GRAVITY * deltaTime;

    collisions(base, keys, deltaTime);

    // Moving towards player's position
    Vect<float> offset = getOffset();
    renderOffset -= (renderOffset - offset) * CAM_TIGHTNESS * deltaTime;
    renderOffset.lock(base.getSize().cast<float>() - Vect<float>(WIN_WIDTH, WIN_HEIGHT), Vect<float>(0.0f, 0.0f));
}

void Player::render(Window& window, Vect<int64_t> renderOffset)
{
    SDL_Rect renderTo = rect;
    renderTo.x -= renderOffset.x;
    renderTo.y -= renderOffset.y;

    window.drawRect(renderTo, {255, 255, 255});
}

std::string Player::getSave()
{
    std::string save = saveName;

    save += std::to_string(pos.x)      + "," + 
            std::to_string(pos.y)      + "," +
            std::to_string(velocity.x) + "," + 
            std::to_string(velocity.y) + "," +
            std::to_string(jump)       + "," +
            std::to_string(canJump);
    
    return save;
}

void Player::readSave(std::string save)
{
    save = save.substr(saveName.length()); // Skipping "Player"

    // Copying over save data
    std::vector<std::string> data = util::split(save, ",");
    
    pos.x      = std::stof(data[0]); 
    pos.y      = std::stof(data[1]);
    velocity.x = std::stof(data[2]); 
    velocity.y = std::stof(data[3]);
    jump       = std::stoi(data[4]); 
    canJump    = std::stoi(data[5]);

    renderOffset = getOffset(); // Updating renderOffset after pos is set
}

void Player::updateRect()
{
    rect = {static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y), size.x,size.y};
}

Vect<float> Player::getOffset() // Percise offset for rendering the player, not moving one
{
    return pos + (size.cast<float>() / 2) - CAM_OFFSET.cast<float>();
}

void Player::collisions(Base& base, std::unordered_map<SDL_Keycode, bool>& keys, float deltaTime)
{
    // Managing downCountdown
    if (keys[SDLK_s] && downCountdown == 0.0f)
        downCountdown = downCountdownMax;
    if (downCountdown > 0.0f) downCountdown -= deltaTime;
    else downCountdown = 0.0f;

    // Collisions with platforms
    if (velocity.y > 0 && !keys[SDLK_s] && downCountdown == 0.0f)
    {
        float origY = pos.y;
        bool onPlatform = false;
        // Going through every pixel moved
        for (; pos.y <= origY + ceil(velocity.y * deltaTime); pos.y++)
        {
            updateRect();
            
            int32_t result = platformCollide(base.getObjects(), deltaTime);
            if (result != -1)
            {
                pos.y = result;
                onPlatform = true;
                break;
            }
        }

        if (!onPlatform) // Resetting
        {
            pos.y = origY + velocity.y * deltaTime;
            canJump = false;
        }
    }
    else 
    {
        canJump = false;
        pos.y += velocity.y * deltaTime;
    }
          
    pos.x += velocity.x;

    pos.lock(base.getSize().cast<float>() - size.cast<float>(), Vect<float>(0.0f, 0.0f));

    // Landed on bottom
    if (pos.y >= (float)base.getSize().y - size.y)
    {
        velocity.y = 0;
        jump = false;
        canJump = true;
    }
    else if (pos.y <= 0) velocity.y = 0;

    updateRect();
}

int32_t Player::platformCollide(std::vector<std::unique_ptr<Interactable>>& objects, float deltaTime)
{
    for (std::unique_ptr<Interactable>& obj : objects)
        if (obj->getType() == Platform_T && !obj->isPlacing())
            if ((pos.y + size.y) <= (obj->getRect().y + 2))
                if (util::collide(rect, obj->getRect()))
                {
                    velocity.y = 0;
                    jump = false;
                    canJump = true;
                    return obj->getRect().y - size.y;
                }
    
    return -1;
}