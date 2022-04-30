#include "player.h"

#include <iostream>
#include <unordered_map>
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

void Player::update(std::unordered_map<SDL_Keycode, bool>& keys,
                    Base& base,
                    float deltaTime)
{
    pos.x += (keys[SDLK_d] - keys[SDLK_a]) * SPEED * deltaTime;

    if (keys[SDLK_w] && !jump)
    {
        velocity.y += JUMP_SPEED;
        jump = true;
    }

    velocity.y -= GRAVITY * deltaTime;
    pos.y += velocity.y * deltaTime;

    collisions(base);

    // Moving towards player's position
    Vect<float> offset = getOffset(); 
    renderOffset.x -= (renderOffset.x - offset.x) * CAM_TIGHTNESS * deltaTime;
    renderOffset.y -= (renderOffset.y - offset.y) * CAM_TIGHTNESS * deltaTime;
}

void Player::render(Window& window, Vect<int> renderOffset)
{
    SDL_Rect renderTo = {
        static_cast<int>(pos.x) - renderOffset.x,
        static_cast<int>(pos.y) - renderOffset.y,
        size.x, 
        size.y
    };

    window.drawRect(renderTo, {255, 255, 255});
}

Vect<float> Player::getOffset() // Percise offset for rendering the player, not moving one
{
    return Vect<float>(static_cast<float>(pos.x + size.x / 2 - WIN_WIDTH / 2 ), 
                       static_cast<float>(pos.y + size.y / 2 - WIN_HEIGHT / 2));
}

void Player::collisions(Base& base)
{
    util::lock(pos.x, (float)base.getSize().x, 0.0f);
    util::lock(pos.y, (float)base.getSize().y, 0.0f);

    // Landed on surface
    if (pos.y == (float)base.getSize().y)
    {
        velocity.y = 0;
        jump = false;
    }

    // Other collision code here
}