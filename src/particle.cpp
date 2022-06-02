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

void Particle::update(Window& window)
{
    const float deltaTime = window.getDeltaTime();

    angle += data.rotationSpeed * deltaTime;

    pos.x += data.speed * cos(moveAngle * M_PI / 180) * deltaTime;
    pos.y += data.speed * sin(moveAngle * M_PI / 180) * deltaTime;
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