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

void Particle::update(Window& window, 
                      const Vect<int64_t> renderOffset, 
                      const Vect<uint32_t> baseSize)
{
    const float deltaTime = window.getDeltaTime();

    // Angle move
    angle += data.rotationSpeed * deltaTime;

    // Movement
    pos.x += data.speed * cos(moveAngle * M_PI / 180) * deltaTime;
    pos.y += data.speed * sin(moveAngle * M_PI / 180) * deltaTime;

    // Wrapping back around to the other side
    const SDL_Rect rect = { pos.xCast<int>(),  pos.yCast<int>(), 
                            size.xCast<int>(), size.yCast<int>() };
    // Move directions (-1 = left, 0 = none, 1 = right)
    const Vect<uint32_t> direction = offscreenDirs(rect, baseSize.cast<int>(), getEdge(rect));
    if (direction != Vect<uint32_t>(0, 0))
    {
        Vect<float> newPos = pos;
        Vect<uint32_t> windowMove = baseSize + (size + getEdge(rect).cast<uint32_t>()) * 2;
        newPos -= (windowMove * direction.cast<uint32_t>()).cast<float>();
        baseSize.print();
        setRenderRect(newPos, renderOffset);
    }
}

void Particle::render(Window& window, const Vect<int64_t> renderOffset)
{
    SDL_Rect dst = getRenderRect(renderOffset);
    
    // If not offscreen
    if (offscreenDirs(dst, window.getSize().cast<int>(), getEdgeScaled(dst)) == Vect<uint32_t>(0, 0))
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

void Particle::setRenderRect(const Vect<float> newPos, const Vect<int64_t> renderOffset)
{
    pos = newPos + (renderOffset.cast<float>() / data.parallax);
}

const Vect<int> Particle::getEdge(const SDL_Rect rect) 
{
    return (size.cast<float>() * ROTATE_EDGES).cast<int>(); 
}

const Vect<int> Particle::getEdgeScaled(const SDL_Rect rect) 
{
    return getEdge(rect) * data.scale; 
}

const Vect<uint32_t> Particle::offscreenDirs(const SDL_Rect rect, const Vect<int> size, const Vect<int> edge)
{
    Vect<uint32_t> offDir(0, 0);

    if (rect.x - edge.x > size.x) offDir.x = 1;
    else if (rect.x + rect.w + edge.x < 0) offDir.x = -1;

    if (rect.y - edge.y > size.y) offDir.y = 1;
    else if (rect.y + rect.h + edge.y < 0) offDir.y = -1;
    
    return offDir;
}