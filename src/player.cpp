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
    : pos(pos), size{20, 20}, renderOffset{0, 0}, velocity{0, 0}, jump(false), canJump(false)
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

void Player::render(Window& window, Vect<int> renderOffset)
{
    SDL_Rect renderTo = rect;
    renderTo.x -= renderOffset.x;
    renderTo.y -= renderOffset.y;

    window.drawRect(renderTo, {255, 255, 255});
}

std::string Player::getSave()
{
    std::string save = saveName + " ";
    save += std::to_string(pos.x) + "," + std::to_string(pos.y) + " ";
    save += std::to_string(velocity.x) + "," + std::to_string(velocity.y) + " ";
    save += std::to_string(jump) + "," + std::to_string(canJump);
    return save;
}

void Player::readSave(std::string save)
{
    save = save.substr(saveName.length() + 1); // Skipping "Player "

    // Copying over save data
    std::vector<std::string> saveData = util::split(save, " ");
    std::vector<std::string> posPart = util::split(saveData[0], ",");
    pos.x = std::stof(posPart[0]); pos.y = std::stof(posPart[1]);
    std::vector<std::string> velPart = util::split(saveData[1], ",");
    velocity.x = std::stof(velPart[0]); velocity.y = std::stof(velPart[1]);
    std::vector<std::string> otherPart = util::split(saveData[2], ",");
    jump = std::stoi(otherPart[0]); canJump = std::stoi(otherPart[1]);

    renderOffset = getOffset();
}

void Player::updateRect()
{
    rect = {static_cast<int>(pos.x), static_cast<int>(pos.y), size.x,size.y};
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
            
            int result = platformCollide(base.getObjects(), deltaTime);
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

int Player::platformCollide(std::vector<std::unique_ptr<Interactable>>& objects, float deltaTime)
{
    for (std::unique_ptr<Interactable>& obj : objects)
        if (obj->getType() == PlatformType && !obj->isPlacing())
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