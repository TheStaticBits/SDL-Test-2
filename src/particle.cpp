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
    const Vect<uint32_t> winSize = window.getSize().cast<uint32_t>();
    
    if (inBox(dst, winSize) == Vect<int32_t>(0, 0))
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

Vect<int32_t> Particle::inBox(const SDL_Rect rect, Vect<uint32_t> uSize)
{
    const Vect<int> size = uSize.cast<int>();
    const Vect<int> sizeVect = { rect.w, rect.h };
    const Vect<int> edge = (sizeVect.cast<float>() * ROTATE_EDGES).cast<int>();

    Vect<int32_t> dirOut(0, 0);

    dirOut.x = (int32_t)(rect.x - edge.x > size.x) - 
               (int32_t)(rect.x + rect.w + edge.x < 0);
    dirOut.y = (int32_t)(rect.y - edge.y > size.y) - 
               (int32_t)(rect.y + rect.h + edge.y < 0);

    return dirOut;
}

void Particle::wrap(Window& window, const Vect<uint32_t> baseSize)
{
    const SDL_Rect rect = getRenderRect({ 0, 0 });
    
    Vect<uint32_t> winSize = window.getSize();
    if (winSize <= baseSize) winSize = baseSize;

    // Bottom right corner
    const Vect<uint32_t> maxRenderOffset = baseSize - winSize;
    // Area that the particle can be in while visible
    const Vect<uint32_t> particleMovArea = winSize + (maxRenderOffset.cast<float>() / data.parallax).cast<uint32_t>();

    const Vect<uint32_t> scaledSize = (Vect<int>( rect.w, rect.h )).cast<uint32_t>();
    // Edge distance to allow room for the 
    // corner of the image while rotation
    const Vect<uint32_t> edge = (scaledSize.cast<float>() * ROTATE_EDGES).cast<uint32_t>();

    // moving down/right = 1, left/up = -1
    const Vect<int32_t> moveAngle = { (int32_t)(cos(this->moveAngle * M_PI / 180) < 0.0f) * 2 - 1,
                                       (int32_t)(sin(this->moveAngle * M_PI / 180) < 0.0f) * 2 - 1 };


    // Distance to travel if it needs to wrap
    const Vect<uint32_t> travelDist = particleMovArea + ((scaledSize + edge));
    Vect<int32_t> dirOut = inBox(rect, particleMovArea);

    // If the particle is offscreen, but moving towards the screen,
    // cancel that wrap around
    if (dirOut.x == moveAngle.x) dirOut.x = 0;
    if (dirOut.y == moveAngle.y) dirOut.y = 0;

    // Wrapping
    pos -= (dirOut * travelDist.cast<int32_t>()).cast<float>();
}