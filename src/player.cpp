#include "player.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"

Player::Player(Vect<float> pos)
    : pos(pos), size{20, 20}, renderOffset(getOffset())
{

}

Player::~Player()
{
    // Destroy image/animations (currently none)
}

void Player::update(std::unordered_map<SDL_Keycode, bool> keys)
{
    pos.x += (keys[SDLK_d] - keys[SDLK_a]) * speed;
    pos.y += (keys[SDLK_s] - keys[SDLK_w]) * speed;

    // Moving towards player's position
    Vect<float> offset = getOffset();
    renderOffset.x -= (renderOffset.x - offset.x) / 25;
    renderOffset.y -= (renderOffset.y - offset.y) / 25;
}

void Player::render(Window& window, Vect<int> renderOffset)
{
    SDL_Rect renderTo = {
        static_cast<int>(pos.x) - renderOffset.x,
        static_cast<int>(pos.y) - renderOffset.y,
        size.x, 
        size.y
    };

    std::cout << "Rendering player at " << pos.x << ", " << pos.y << std::endl;
    std::cout << "renderOffset: " << renderOffset.x << ", " << renderOffset.y << std::endl;

    window.drawRect(renderTo, {255, 255, 255});
}

Vect<float> Player::getOffset() // Percise offset for rendering the player, not moving one
{
    return Vect<float>(static_cast<float>(pos.x + size.x / 2 - WIN_WIDTH / 2 ), 
                       static_cast<float>(pos.y + size.y / 2 - WIN_HEIGHT / 2));
}