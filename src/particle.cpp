#include "particle.h"

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"

Particle::Particle(SDL_Texture* texture, 
                   const Vect<float> startPos, 
                   const float startAngle, const float moveAngle, 
                   const ParticleData& data)
    : texture(texture), pos(startPos), angle(startAngle),
      moveAngle(moveAngle), data(data),
      size(util::getSize(texture))
{

}

Particle::~Particle()
{
    // Delete texture elsewhere
}

void Particle::update(Window& window, const Vect<uint32_t> baseSize)
{
    const float deltaTime = window.getDeltaTime();

    angle += data.rotationSpeed * deltaTime;

    pos.x += data.speed * cos(moveAngle * M_PI / 180) * deltaTime;
    pos.y += data.speed * sin(moveAngle * M_PI / 180) * deltaTime;

    wrap(window, baseSize);
}

void Particle::render(Window& window, const Vect<int64_t> renderOffset)
{
    SDL_Rect dst = getRenderRect(renderOffset);
    const Vect<int> winSize = window.getSize().cast<int>();
    
    if (inBox(dst, winSize))
    {
        window.modColor(texture, data.color);
        window.render(texture, dst, angle);
    }
}

SDL_Rect Particle::getRenderRect(const Vect<int64_t> renderOffset) const
{
    // xtreme casts
    const Vect<int> posInt = (pos - (renderOffset.cast<float>() / data.parallax)).cast<int>();
    const Vect<int> sizeInt = (size.cast<float>() * data.scale).cast<int>();

    return { posInt.x, posInt.y, sizeInt.x, sizeInt.y };
}

bool Particle::inBox(const SDL_Rect rect, const Vect<int> size)
{
    const Vect<int> sizeVect = { rect.w, rect.h };
    const Vect<int> edge = (sizeVect.cast<float>() * ROTATE_EDGES).cast<int>();

    return (rect.x - edge.x < size.x) && (rect.y - edge.y < size.y) &&
           (rect.x + rect.w + edge.x > 0) && (rect.y + rect.h + edge.y > 0);
}

void Particle::wrap(Window& window, const Vect<uint32_t> baseSize)
{
    // could scale down position by parallax and check off screen (divided by parallax), move scaled down size, and then scale back up!

    // Wrap around if offscreen
    const SDL_Rect topLeft = getRenderRect(Vect<int64_t>(0, 0));
    const SDL_Rect bottomRight = getRenderRect((baseSize - window.getSize()).cast<int64_t>());
    const Vect<int> renderSize(topLeft.w, topLeft.h);
    const Vect<int> bSizeShrunk = (baseSize.cast<float>() / data.parallax).cast<int>();

    const Vect<int> edge = (size.cast<float>() * data.scale * ROTATE_EDGES).cast<int>();

    // hacky here, "* 4" and line 83, "*= 2"
    Vect<float> travelDist = bSizeShrunk.cast<float>() + ((renderSize + edge) * 4).cast<float>();
    travelDist.x *= 2;

    // Top left corner
    if (topLeft.x + topLeft.w + edge.x < 0) pos.x += travelDist.x;
    if (topLeft.y + topLeft.h + edge.y < 0) pos.y += travelDist.y;

    // Bottom right corner
    if (bottomRight.x - bottomRight.w - edge.x > bSizeShrunk.x) pos.x -= travelDist.x;
    if (bottomRight.y - bottomRight.h - edge.y > bSizeShrunk.y) pos.y -= travelDist.y;
}