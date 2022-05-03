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
    : pos(pos), size{20, 20}, renderOffset(getOffset()), velocity{0, 0}, jump(false)
{

}

Player::~Player()
{
    // Destroy image/animations (currently none)
}

void Player::update(std::unordered_map<SDL_Keycode, bool>& keys, Base& base, float deltaTime)
{
    velocity.x = (keys[SDLK_d] - keys[SDLK_a]) * SPEED * deltaTime;

    if (keys[SDLK_w] && !jump)
    {
        velocity.y += JUMP_SPEED;
        jump = true;
    }

    velocity.y -= GRAVITY * deltaTime;

    collisions(base, keys, deltaTime);

    // Moving towards player's position
    Vect<float> offset = getOffset(); 
    renderOffset.x -= (renderOffset.x - offset.x) * CAM_TIGHTNESS * deltaTime;
    renderOffset.y -= (renderOffset.y - offset.y) * CAM_TIGHTNESS * deltaTime;
}

void Player::render(Window& window, Vect<int> renderOffset)
{
    SDL_Rect renderTo = rect;
    renderTo.x -= renderOffset.x;
    renderTo.y -= renderOffset.y;

    window.drawRect(renderTo, {255, 255, 255});
}

void Player::updateRect()
{
    rect = {static_cast<int>(pos.x), static_cast<int>(pos.y), size.x,size.y};
}

Vect<float> Player::getOffset() // Percise offset for rendering the player, not moving one
{
    return Vect<float>(static_cast<float>(pos.x + size.x / 2 - WIN_WIDTH / 2 ), 
                       static_cast<float>(pos.y + size.y / 2 - WIN_HEIGHT / 1.5));
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
            pos.y = origY + velocity.y * deltaTime;
    }
    else pos.y += velocity.y * deltaTime;
          
    pos.x += velocity.x;
    
    util::lock(pos.x, (float)base.getSize().x - size.x, 0.0f);
    util::lock(pos.y, (float)base.getSize().y - size.y, 0.0f);

    // Landed on bottom
    if (pos.y >= (float)base.getSize().y - size.y)
    {
        velocity.y = 0;
        jump = false;
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
                    return obj->getRect().y - size.y;
                }
    
    return -1;
}