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

Player::Player(const Window& window)
    : pos(Vect<float>(window.getCamSize().xCast<float>() / 2, window.getCamSize().yCast<float>())), 
      size{TILE_SIZE, TILE_SIZE}, renderOffset(getOffset(window)),
      velocity{0, 0}, jump(false), canJump(false)
{

}

Player::~Player()
{
    // Destroy image/animations (currently none)
}

void Player::update(Window& window, Base& base)
{
    const float deltaTime = window.getDeltaTime();

    velocity.x = (window.pKey(SDLK_d) - window.pKey(SDLK_a)) * SPEED * deltaTime;

    if (window.pKey(SDLK_w) && !jump && canJump)
    {
        velocity.y += JUMP_SPEED;
        jump = true;
        canJump = false;
    }

    velocity.y -= GRAVITY * deltaTime;

    collisions(window, base, deltaTime);

    // Moving towards player's position
    Vect<float> offset = getOffset(window);
    renderOffset -= (renderOffset - offset) * CAM_TIGHTNESS * deltaTime;
    lockOffset(base, window);
}

void Player::render(Window& window, const Vect<int64_t> renderOffset)
{
    SDL_Rect renderTo = rect;
    renderTo.x -= renderOffset.xCast<int>();
    renderTo.y -= renderOffset.yCast<int>();

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

void Player::readSave(std::string save, const Window& window)
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

    renderOffset = getOffset(window); // Updating renderOffset after pos is set
}

void Player::updateRect()
{
    rect = util::getRect(pos.cast<int64_t>(), size);
}

void Player::resize(Base& base, const Window& window)
{
    renderOffset = getOffset(window);
    lockOffset(base, window);
}

Vect<float> Player::getOffset(const Window& window) // Percise offset for rendering the player, not moving one
{
    Vect<int32_t> camOffset = {(int32_t)round(window.getCamSize().x / 2), 
                               (int32_t)round(window.getCamSize().y / 1.5)};
    return pos + (size.cast<float>() / 2) - camOffset.cast<float>();
}

void Player::lockOffset(Base& base, const Window& window)
{
    renderOffset.lock(base.getSize().cast<float>() - window.getCamSize().cast<float>(), Vect<float>(0.0f, 0.0f));
}

void Player::collisions(Window& window, Base& base, const float deltaTime)
{
    // Managing downCountdown
    if (window.pKey(SDLK_s) && downCountdown == 0.0f)
        downCountdown = downCountdownMax;
    if (downCountdown > 0.0f) downCountdown -= deltaTime;
    else downCountdown = 0.0f;

    // Collisions with platforms
    if (velocity.y > 0 && !window.pKey(SDLK_s) && downCountdown == 0.0f)
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
                pos.y = static_cast<float>(result);
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