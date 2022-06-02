#include "particle.h"

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"

Particle::Particle(SDL_Texture* texture, 
                   const Vect<int64_t> startPos, 
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
    window.modColor(texture, data.color);

    Vect<int> posInt = (pos - (renderOffset / data.parallax)).cast<int>();
    Vect<int> sizeInt = (size * data.scale).cast<int>();
    SDL_Rect dst = { posInt.x, posInt.y, sizeInt.x, sizeInt.y };

    window.render(texture, dst, angle);
}